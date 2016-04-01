#include "IllustratorSDK.h"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"

#include "AICSXS.h"

#include "ColorToolsUIController.h"
#include "BtSwatchList.h"
#include "BtAiMenuItem.h"
#include "BtAiMenuItemHandles.h"
#include "FixBlack.h"

#include "TextTools.h"
#include "FixFreehandType.h"
#include "DictFuncs.h"

SafeguardToolsPlugin *gPlugin = NULL;

Plugin* AllocatePlugin(SPPluginRef pluginRef)
{
	return new SafeguardToolsPlugin(pluginRef);
}

void FixupReload(Plugin* plugin)
{
	SafeguardToolsPlugin::FixupVTable((SafeguardToolsPlugin*) plugin);
}

SafeguardToolsPlugin::SafeguardToolsPlugin(SPPluginRef pluginRef) :
	Plugin(pluginRef),
    fRegisterEventNotifierHandle(NULL),
    colorToolsUIController(NULL),
    mySwatchList(NULL)
{
	strncpy(fPluginName, kSafeguardToolsPluginName, kMaxStringLength);
}

SafeguardToolsPlugin::~SafeguardToolsPlugin()
{
}

ASErr SafeguardToolsPlugin::Message(char* caller, char* selector, void *message)
{
	ASErr error = kNoErr;
    
	try {
		error = Plugin::Message(caller, selector, message);
	}
	catch (ai::Error& ex) {
		error = ex;
	}
	catch (...) {
		error = kCantHappenErr;
	}
	if (error) {
		if (error == kUnhandledMsgErr) {
			// Defined by Plugin.hpp and used in Plugin::Message - ignore.
			error = kNoErr;
		}
		else {
			Plugin::ReportError(error, caller, selector, message);
		}
	}
	return error;
}

ASErr SafeguardToolsPlugin::SetGlobal(Plugin *plugin)
{
    gPlugin = (SafeguardToolsPlugin *) plugin;
    return kNoErr;
}


ASErr SafeguardToolsPlugin::StartupPlugin( SPInterfaceMessage *message )
{
	ASErr error = kNoErr;
    error = Plugin::StartupPlugin(message);
    if (error) { return error; }
    
    if (NULL == colorToolsUIController)
    {
        colorToolsUIController = new ColorToolsUIController();
        
        error = Plugin::LockPlugin(true);
        if (error) { return error; }
    }
    
    if (NULL == mySwatchList)
    {
        mySwatchList = new BtSwatchList();
    }
    
    error = this->AddMenus(message);
    if (error) { return error; }
    
    //Register for notifiers
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAICSXSPlugPlugSetupCompleteNotifier, &fRegisterEventNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAIApplicationStartedNotifier, &fAppStartedNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAIDocumentOpenedNotifier, &fDocOpenedNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAIArtCustomColorChangedNotifier, &fCustomColorChangeNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAISwatchLibraryChangedNotifier, &fSwatchLibChangeNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAIArtSelectionChangedNotifier, &fArtSelectionChangeNotifierHandle);
    if (error) { return error; }
    
    return error;
}

ASErr SafeguardToolsPlugin::ShutdownPlugin( SPInterfaceMessage *message )
{
    if (colorToolsUIController)
    {
        colorToolsUIController->RemoveEventListeners();
        delete colorToolsUIController;
        colorToolsUIController = NULL;
        Plugin::LockPlugin(false);
    }
    if (mySwatchList)
    {
        delete mySwatchList;
    }

    message->d.globals = NULL;
    return Plugin::ShutdownPlugin(message);

}

ASErr SafeguardToolsPlugin::ReloadPlugin(SPInterfaceMessage *message)
{
    return Plugin::ReloadPlugin(message);
}

ASErr SafeguardToolsPlugin::UnloadPlugin(SPInterfaceMessage *message)
{
    return Plugin::UnloadPlugin(message);
}

/*
 */
ASErr SafeguardToolsPlugin::PostStartupPlugin()
{
    return kNoErr;
}

/*
 */
ASErr SafeguardToolsPlugin::AddMenus(SPInterfaceMessage* message)
{
    AIPlatformAddMenuItemDataUS menuItem;
    const char* modifySwatchesGroup = MODIFY_SWATCHES_MENU;
    
    menuItem.groupName = kEditMenuGroup;
    menuItem.itemText = ai::UnicodeString(MODIFY_SWATCHES_MENU);
    BtAiMenuItem* ModifySwatchesMenu = new BtAiMenuItem(menuItem, kMenuGroupSortedAlphabeticallyOption);
    
    menuItem.groupName = modifySwatchesGroup;
    menuItem.itemText = ai::UnicodeString(FIX_BLACK_MENU_ITEM);
    BtAiMenuItem* FixBlackMenuItem = new BtAiMenuItem(menuItem, kMenuItemNoOptions);
    ModifySwatchesMenu->AddSubMenuItem(*FixBlackMenuItem);
    
    menuItem.groupName = modifySwatchesGroup;
    menuItem.itemText = ai::UnicodeString(FIND_AND_REPLACE_MENU_ITEM);
    BtAiMenuItem* FindAndReplaceGraphics = new BtAiMenuItem(menuItem, kMenuItemWantsUpdateOption);
    ModifySwatchesMenu->AddSubMenuItem(*FindAndReplaceGraphics);
    
    BtAiMenuItem::AddMenu(*ModifySwatchesMenu, &menuItemHandles);
	
    return kNoErr;
/*    //*********** Text Tools
    {
        AIPlatformAddMenuItemDataUS textToolsMenuData;
        const char *textToolsMenuGroupCStr = "SDKTextToolsGroup";
        const char *textToolsMenuCStr = "Text Tools";
        AIPlatformAddMenuItemDataUS convertToPointTypeMenuData;
        const char *convertToPointTypeCStr = "Make Point Type";
        AIPlatformAddMenuItemDataUS fixFreehandMenuData;
        const char *fixFreehandTypeCStr = "Fix Freehand Type";
        
        //This line tells AI to put the text tools menu in the Type menu
        textToolsMenuData.groupName = kTypeLayoutMenuGroup;
        textToolsMenuData.itemText = ai::UnicodeString(textToolsMenuCStr);
        
        convertToPointTypeMenuData.groupName = textToolsMenuGroupCStr;
        convertToPointTypeMenuData.itemText = ai::UnicodeString(convertToPointTypeCStr);
        fixFreehandMenuData.groupName = textToolsMenuGroupCStr;
        fixFreehandMenuData.itemText = ai::UnicodeString(fixFreehandTypeCStr);
        
        //Check for Text Tools Menu and add menu items
        if ( !SDKGroupAlreadyMade(textToolsMenuGroupCStr) ) {
            AIMenuItemHandle dummyItem;
            error = sAIMenu->AddMenuItem( message->d.self, textToolsMenuGroupCStr, &textToolsMenuData, 0, &dummyItem );
            if ( error ) goto error;
            
            error = sAIMenu->AddMenuGroupAsSubMenu( textToolsMenuGroupCStr, kMenuGroupSortedAlphabeticallyOption, dummyItem, &menuGroup );
            if ( error ) goto error;
        }
        
        error = sAIMenu->AddMenuItem( message->d.self, convertToPointTypeCStr, &convertToPointTypeMenuData, kMenuItemNoOptions, &ConvertToPointTypeMenuItemSelected );
        sAIMenu->UpdateMenuItemAutomatically(ConvertToPointTypeMenuItemSelected, kAutoEnableMenuItemAction, 0, 0, kIfText, 0, 0, 0);
        if ( error ) goto error;
        
        error = sAIMenu->AddMenuItem( message->d.self, fixFreehandTypeCStr, &fixFreehandMenuData, kMenuItemNoOptions, &FixFreehandTypeMenuItemSelected );
        sAIMenu->UpdateMenuItemAutomatically(FixFreehandTypeMenuItemSelected, kAutoEnableMenuItemAction, 0, 0, kIfText, 0, 0, 0);
        if ( error ) goto error;
    }
    
    //************* Align
    {
        AIPlatformAddMenuItemDataUS alignMenuData;
        const char *alignMenuGroupCStr = "SDKAlignGroup";
        const char *alignMenuCStr = "Align";
        //Add the align menu to the objects menu
        alignMenuData.groupName = kObjectAttribsMenuGroup;
        alignMenuData.itemText = ai::UnicodeString(alignMenuCStr);
        
        //****Align Left
        AIPlatformAddMenuItemDataUS alignLeftMenuData;
        const char *alignLeftCStr = "Left";
        alignLeftMenuData.groupName = alignMenuGroupCStr;
        alignLeftMenuData.itemText = ai::UnicodeString(alignLeftCStr);
        
        //****Align Center
        AIPlatformAddMenuItemDataUS alignCenterMenuData;
        const char *alignCenterCStr = "Center";
        alignCenterMenuData.groupName = alignMenuGroupCStr;
        alignCenterMenuData.itemText = ai::UnicodeString(alignCenterCStr);
        
        //****Align Right
        AIPlatformAddMenuItemDataUS alignRightMenuData;
        const char *alignRightCStr = "Right";
        alignRightMenuData.groupName = alignMenuGroupCStr;
        alignRightMenuData.itemText = ai::UnicodeString(alignRightCStr);
        
        //Check for Align Menu and add menu items
        if ( !SDKGroupAlreadyMade(alignMenuGroupCStr) ) {
            AIMenuItemHandle dummyItem;
            error = sAIMenu->AddMenuItem( message->d.self, alignMenuGroupCStr, &alignMenuData, 0, &dummyItem );
            if ( error ) goto error;
            
            error = sAIMenu->AddMenuGroupAsSubMenu( alignMenuGroupCStr, kMenuGroupSortedAlphabeticallyOption, dummyItem, &menuGroup );
            if ( error ) goto error;
        }
        
        error = sAIMenu->AddMenuItem( message->d.self, alignLeftCStr, &alignLeftMenuData, kMenuItemIgnoreSort, &AlignLeftMenuItemSelected );
        sAIMenu->UpdateMenuItemAutomatically(AlignLeftMenuItemSelected, kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
        if ( error ) goto error;
        
        error = sAIMenu->AddMenuItem( message->d.self, alignCenterCStr, &alignCenterMenuData, kMenuItemIgnoreSort, &AlignCenterMenuItemSelected );
        sAIMenu->UpdateMenuItemAutomatically(AlignCenterMenuItemSelected, kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
        if ( error ) goto error;
        
        error = sAIMenu->AddMenuItem( message->d.self, alignRightCStr, &alignRightMenuData, kMenuItemIgnoreSort, &AlignRightMenuItemSelected );
        sAIMenu->UpdateMenuItemAutomatically(AlignRightMenuItemSelected, kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
        if ( error ) goto error;
    }
    
    //************* MICR BARCODE
    {
        AIPlatformAddMenuItemDataUS micrbarcodeMenuData;
        const char *micrbarcodeMenuGroupCStr = "SDKMICRGroup";
        const char *micrbarcodeMenuCStr = "MICR Barcode";
        //Add the micr barcode menu to the objects menu
        micrbarcodeMenuData.groupName = kTypePluginsMenuGroup1;
        micrbarcodeMenuData.itemText = ai::UnicodeString(micrbarcodeMenuCStr);
        
        //****Create MICR Barcode
        AIPlatformAddMenuItemDataUS createMicrbarcodeMenuData;
        const char *createMicrbarcodeCStr = "Create MICR Barcode";
        createMicrbarcodeMenuData.groupName = micrbarcodeMenuGroupCStr;
        createMicrbarcodeMenuData.itemText = ai::UnicodeString(createMicrbarcodeCStr);
        
        //Check for Align Menu and add menu items
        if ( !SDKGroupAlreadyMade(micrbarcodeMenuGroupCStr) ) {
            AIMenuItemHandle dummyItem;
            error = sAIMenu->AddMenuItem( message->d.self, micrbarcodeMenuGroupCStr, &micrbarcodeMenuData, 0, &dummyItem );
            if ( error ) goto error;
            
            error = sAIMenu->AddMenuGroupAsSubMenu( micrbarcodeMenuGroupCStr, kMenuGroupSortedAlphabeticallyOption, dummyItem, &menuGroup );
            if ( error ) goto error;
        }
        
        error = sAIMenu->AddMenuItem( message->d.self, createMicrbarcodeCStr, &createMicrbarcodeMenuData, kMenuItemIgnoreSort|kMenuItemWantsUpdateOption, &CreateMICRBarcodeMenuItemSelected );
        if ( error ) goto error;
    }
 */
}


/*
 */
ASErr SafeguardToolsPlugin::GoMenuItem(AIMenuMessage* message)
{
	AIErr error = kNoErr;
	
	if ( message->menuItem == menuItemHandles.GetHandleWithKey(FIX_BLACK_MENU_ITEM) )
	{
        FixBlack();
    }
	if ( message->menuItem == menuItemHandles.GetHandleWithKey(FIND_AND_REPLACE_MENU_ITEM) )
	{
        AIBoolean state;
        sAICSXSExtension->IsPrimaryStageVisible(COLORTOOLS_UI_EXTENSION, state);

        if (state == TRUE)
        {
            colorToolsUIController->SendCloseMessageToHtml();
        }
        else
        {
            colorToolsUIController->LoadExtension();
            sAICSXSExtension->LaunchExtension(COLORTOOLS_UI_EXTENSION);
        }

	}
    
    if ( message->menuItem == ConvertToPointTypeMenuItemSelected )
    {
        //Call the main function
        if ( ConvertToPointType() ) {
            //What to do if it worked.
        }
    } else if ( message->menuItem == FixFreehandTypeMenuItemSelected )
    {
        //Call the main function
        if ( FixFreehandType() ) {
            //What to do if it worked.
        }
    } else if ( message->menuItem == AlignLeftMenuItemSelected )
    {
        //Call the main function
        if ( Align(ATE::kLeftJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    } else if ( message->menuItem == AlignCenterMenuItemSelected )
    {
        //Call the main function
        if ( Align(ATE::kCenterJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    } else if ( message->menuItem == AlignRightMenuItemSelected )
    {
        //Call the main function
        if ( Align(ATE::kRightJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    } else if ( message->menuItem == CreateMICRBarcodeMenuItemSelected )
    {
        //Call the main function
        if ( CreateMICRBarcode() ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Create MICR Barcode"), ai::UnicodeString("Redo Create MICR Barcode"));
        }
    }
	
	if (error)
		goto error;
	
error:
	return error;
}


ASErr SafeguardToolsPlugin::UpdateMenuItem(AIMenuMessage* message)
{
	AIErr error = kNoErr;
	
	if (message->menuItem == menuItemHandles.GetHandleWithKey(FIND_AND_REPLACE_MENU_ITEM) ) {
        AIBoolean state;
        sAICSXSExtension->IsPrimaryStageVisible(COLORTOOLS_UI_EXTENSION, state);
		if (state == TRUE)
		{
			sAIMenu->SetItemText( message->menuItem, ai::UnicodeString("Hide Find and Replace Graphics") );
		}
		else
		{
			sAIMenu->SetItemText( message->menuItem, ai::UnicodeString("Show Find and Replace Graphics") );
		}
	}
    
    //Check if we have a micr line object in the document dictionary
    //If we do, nothing needs to be selected, as we already know where the micr line is
    if ( CheckDictionaryForArtObjectWithIdentifier(MICR_LINE_LABEL) ) {
        sAIMenu->EnableItem(message->menuItem);
    } else {
        //If we couldn't find a micr number in the dictionary, we'll check if some text is selected
        int inArtwork, isSelected, isTrue;
        sAIMenu->GetUpdateFlags(&inArtwork, &isSelected, &isTrue);
        if (isSelected & kIfText) {
            sAIMenu->EnableItem(message->menuItem);
        } else {
            sAIMenu->DisableItem(message->menuItem);
        }
    }

	
	if (error)
		goto error;
	
error:
	return error;
}

ASErr SafeguardToolsPlugin::Notify(AINotifierMessage *message )
{
    if ( message->notifier == fRegisterEventNotifierHandle)
    {
        colorToolsUIController->RegisterCSXSEventListeners();
    }
    
    if ( message->notifier == fAppStartedNotifierHandle ) {
        // Whatever we want to do when the app starts
    }
    
    if (message->notifier == fDocOpenedNotifierHandle ||
        message->notifier == fCustomColorChangeNotifierHandle ||
        message->notifier == fSwatchLibChangeNotifierHandle ) {
        
        string swatchesXml = gPlugin->GetBtSwatchList()->GetColorListAsXMLString();
        colorToolsUIController->SendColorListXmlToHtml(swatchesXml);
    }
    
    if (message->notifier == fArtSelectionChangeNotifierHandle ) {
        colorToolsUIController->DetermineChangeInStatus();
    }
    return kNoErr;
}

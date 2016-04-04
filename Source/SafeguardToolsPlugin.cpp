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
    BtAiMenuItem* FindAndReplaceGraphicsMenuItem = new BtAiMenuItem(menuItem, kMenuItemWantsUpdateOption);
    ModifySwatchesMenu->AddSubMenuItem(*FindAndReplaceGraphicsMenuItem);
    
    BtAiMenuItem::AddMenu(*ModifySwatchesMenu, &menuItemHandles);
    
	
    const char* textToolsGroup = TEXT_TOOLS_MENU;
    
    menuItem.groupName = kTypeLayoutMenuGroup;
    menuItem.itemText = ai::UnicodeString(TEXT_TOOLS_MENU);
    BtAiMenuItem* TextToolsMenu = new BtAiMenuItem(menuItem, kMenuGroupSortedAlphabeticallyOption);
    
    menuItem.groupName = textToolsGroup;
    menuItem.itemText = ai::UnicodeString(MAKE_POINT_TYPE_MENU_ITEM);
    BtAiMenuItem* MakePointTypeMenuItem = new BtAiMenuItem(menuItem, kMenuItemNoOptions);
    MakePointTypeMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfText, 0, 0, 0);
    TextToolsMenu->AddSubMenuItem(*MakePointTypeMenuItem);
    
    menuItem.groupName = textToolsGroup;
    menuItem.itemText = ai::UnicodeString(FIX_FREEHAND_TYPE_MENU_ITEM);
    BtAiMenuItem* FixFreehandTypeMenuItem = new BtAiMenuItem(menuItem, kMenuItemNoOptions);
    FixFreehandTypeMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfText, 0, 0, 0);
    TextToolsMenu->AddSubMenuItem(*FixFreehandTypeMenuItem);
    
    BtAiMenuItem::AddMenu(*TextToolsMenu, &menuItemHandles);
    
    
    const char* alignGroup = ALIGN_MENU;
    
    menuItem.groupName = kObjectAttribsMenuGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_MENU);
    BtAiMenuItem* AlignMenu = new BtAiMenuItem(menuItem, kMenuGroupSortedAlphabeticallyOption);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_LEFT_MENU_ITEM);
    BtAiMenuItem* AlignLeftMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    MakePointTypeMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu->AddSubMenuItem(*AlignLeftMenuItem);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_CENTER_MENU_ITEM);
    BtAiMenuItem* AlignCenterMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    MakePointTypeMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu->AddSubMenuItem(*AlignCenterMenuItem);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_RIGHT_MENU_ITEM);
    BtAiMenuItem* AlignRightMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    MakePointTypeMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu->AddSubMenuItem(*AlignRightMenuItem);
    
    BtAiMenuItem::AddMenu(*AlignMenu, &menuItemHandles);

    
    menuItem.groupName = kTypePluginsMenuGroup1;
    menuItem.itemText = ai::UnicodeString(CREATE_MICR_BARCODE_MENU_ITEM);
    BtAiMenuItem* CreateMicrBarcodeMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort|kMenuItemWantsUpdateOption);
    
    BtAiMenuItem::AddMenu(*CreateMicrBarcodeMenuItem, &menuItemHandles);
    
    return kNoErr;
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
    
    if ( message->menuItem == menuItemHandles.GetHandleWithKey(MAKE_POINT_TYPE_MENU_ITEM) )
    {
        //Call the main function
        if ( ConvertToPointType() ) {
            //What to do if it worked.
        }
    } else if ( message->menuItem == menuItemHandles.GetHandleWithKey(FIX_FREEHAND_TYPE_MENU_ITEM) )
    {
        //Call the main function
        if ( FixFreehandType() ) {
            //What to do if it worked.
        }
    } else if ( message->menuItem == menuItemHandles.GetHandleWithKey(ALIGN_LEFT_MENU_ITEM) )
    {
        //Call the main function
        if ( Align(ATE::kLeftJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    } else if ( message->menuItem == menuItemHandles.GetHandleWithKey(ALIGN_CENTER_MENU_ITEM) )
    {
        //Call the main function
        if ( Align(ATE::kCenterJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    } else if ( message->menuItem == menuItemHandles.GetHandleWithKey(ALIGN_RIGHT_MENU_ITEM) )
    {
        //Call the main function
        if ( Align(ATE::kRightJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    } else if ( message->menuItem == menuItemHandles.GetHandleWithKey(CREATE_MICR_BARCODE_MENU_ITEM) )
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
        ASBoolean check = false;
        sAIMenu->IsItemEnabled(menuItemHandles.GetHandleWithKey(FIND_AND_REPLACE_MENU_ITEM), &check);
        sAIMenu->EnableItem(menuItemHandles.GetHandleWithKey(FIND_AND_REPLACE_MENU_ITEM));
	}
    
    if (message->menuItem == menuItemHandles.GetHandleWithKey(CREATE_MICR_BARCODE_MENU_ITEM) ) {
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
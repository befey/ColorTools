#include "IllustratorSDK.h"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"

#include "AICSXS.h"

#include "ColorToolsUIController.h"
#include "PrintToPdfUIController.h"
#include "BtSwatchList.h"
#include "BtAiMenuItem.h"
#include "BtAiMenuItemHandles.h"
#include "FixBlack.h"

#include "TextTools.h"
#include "FixFreehandType.h"
#include "DictFuncs.h"
#include "PdfPrinter.h"
#include "BleedInfo.h"
#include "ListFonts.h"

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
    
    if (strcmp(caller, kCallerAIPluginGroup) == 0)
    {
        if (strcmp( selector, kSelectorAINotifyEdits ) == 0)
        {
            //error = BleedInfo::PluginGroupNotify((AIPluginGroupMessage *)message);
        }
        else if (strcmp( selector, kSelectorAIUpdateArt ) == 0)
        {
            //error = BleedInfo::PluginGroupUpdate((AIPluginGroupMessage *)message);
        }
    }
    else
    {
        try
        {
            error = Plugin::Message(caller, selector, message);
        }
        catch (ai::Error& ex)
        {
            error = ex;
        }
        catch (...)
        {
            error = kCantHappenErr;
        }
    }
    
    if (error)
    {
        if (error == kUnhandledMsgErr)
        {
            // Defined by Plugin.hpp and used in Plugin::Message - ignore.
            error = kNoErr;
        }
        else
        {
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
        colorToolsUIController = std::make_shared<ColorToolsUIController>();
        
        error = Plugin::LockPlugin(true);
        if (error) { return error; }
    }
    
    if (NULL == printToPdfUIController)
    {
        printToPdfUIController = std::make_shared<PrintToPdf::PrintToPdfUIController>();
        
        error = Plugin::LockPlugin(true);
        if (error) { return error; }
    }
    
    if (NULL == mySwatchList)
    {
        mySwatchList = std::make_shared<BtSwatchList>();
    }
    
    if (NULL == sgJobFile)
    {
        sgJobFile = make_unique<SafeguardJobFile>();
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
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAIDocumentCropAreaModifiedNotifier, &fDocumentCropAreaModifiedNotifierHandle);
    if (error) { return error; }
    
    //Register PlateBleedInfo plugin group
    AIAddPluginGroupData pluginGroupData;
    pluginGroupData.major = 1;
    pluginGroupData.minor = 0;
    pluginGroupData.desc = "__SafeguardPlateInfo__";
    sAIPluginGroup->AddAIPluginGroup (message->d.self,
                                      CREATE_PLATE_BLEED_INFO_PLUGIN_GROUP,
                                      &pluginGroupData,
                                      kPluginGroupKeepWhenEmptyOption | kPluginGroupDoNotTarget | kPluginGroupDoNotSmartTarget | kPluginGroupAskToShowContents,
                                      &bleedInfoPluginGroupHandle);
    
    return error;
}

ASErr SafeguardToolsPlugin::ShutdownPlugin( SPInterfaceMessage *message )
{
    if (colorToolsUIController)
    {
        colorToolsUIController->RemoveEventListeners();
        Plugin::LockPlugin(false);
    }
    if (printToPdfUIController)
    {
        printToPdfUIController->RemoveEventListeners();
        Plugin::LockPlugin(false);
    }
    if (sgJobFile)
    {
        sgJobFile.reset();
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
    //MODIFY SWATCHES MENU
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
    
	
    //TEXT TOOLS MENU
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
    
    
    //ALIGN MENU
    const char* alignGroup = ALIGN_MENU;
    
    menuItem.groupName = kObjectAttribsMenuGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_MENU);
    BtAiMenuItem* AlignMenu = new BtAiMenuItem(menuItem, kMenuGroupSortedAlphabeticallyOption);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_LEFT_MENU_ITEM);
    BtAiMenuItem* AlignLeftMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    AlignLeftMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu->AddSubMenuItem(*AlignLeftMenuItem);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_CENTER_MENU_ITEM);
    BtAiMenuItem* AlignCenterMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    AlignCenterMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu->AddSubMenuItem(*AlignCenterMenuItem);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_RIGHT_MENU_ITEM);
    BtAiMenuItem* AlignRightMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    AlignRightMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu->AddSubMenuItem(*AlignRightMenuItem);
    
    BtAiMenuItem::AddMenu(*AlignMenu, &menuItemHandles);

    
    //CREATE MICR BARCODE
    menuItem.groupName = kTypePluginsMenuGroup1;
    menuItem.itemText = ai::UnicodeString(CREATE_MICR_BARCODE_MENU_ITEM);
    BtAiMenuItem* CreateMicrBarcodeMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort|kMenuItemWantsUpdateOption);
    
    BtAiMenuItem::AddMenu(*CreateMicrBarcodeMenuItem, &menuItemHandles);
    
    
    //List Fonts
    menuItem.groupName = kTypePluginsMenuGroup1;
    menuItem.itemText = ai::UnicodeString(LIST_FONTS_MENU_ITEM);
    BtAiMenuItem* ListFontsMenuItem = new BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    ListFontsMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    
    BtAiMenuItem::AddMenu(*ListFontsMenuItem, &menuItemHandles);
    

    //PRINT TO PDF
    menuItem.groupName = kSaveForMenuGroup;
    menuItem.itemText = ai::UnicodeString(PRINT_TO_PDF_MENU_ITEM);
    BtAiMenuItem* PrintToPdfMenuItem = new BtAiMenuItem(menuItem, kMenuItemNoOptions);
    PrintToPdfMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    
    BtAiMenuItem::AddMenu(*PrintToPdfMenuItem, &menuItemHandles);
    
    
    //CREATE SLUG INFO
    menuItem.groupName = kDocumentUtilsMenuGroup;
    menuItem.itemText = ai::UnicodeString(CREATE_PLATE_BLEED_INFO_MENU_ITEM);
    BtAiMenuItem* CreatePlateBleedInfoMenuItem = new BtAiMenuItem(menuItem, kMenuItemWantsUpdateOption);
    CreatePlateBleedInfoMenuItem->SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    
    BtAiMenuItem::AddMenu(*CreatePlateBleedInfoMenuItem, &menuItemHandles);
 
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
	else if ( message->menuItem == menuItemHandles.GetHandleWithKey(FIND_AND_REPLACE_MENU_ITEM) )
	{
        AIBoolean state;
        sAICSXSExtension->IsPrimaryStageVisible(ColorToolsUIController::COLORTOOLS_UI_EXTENSION, state);

        if (state == TRUE)
        {
            colorToolsUIController->SendCloseMessageToHtml();
        }
        else
        {
            colorToolsUIController->LoadExtension();
            sAICSXSExtension->LaunchExtension(ColorToolsUIController::COLORTOOLS_UI_EXTENSION);
        }

	}
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(MAKE_POINT_TYPE_MENU_ITEM) )
    {
        //Call the main function
        if ( ConvertToPointType() ) {
            //What to do if it worked.
        }
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(FIX_FREEHAND_TYPE_MENU_ITEM) )
    {
        //Call the main function
        if ( FixFreehandType() ) {
            //What to do if it worked.
        }
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(ALIGN_LEFT_MENU_ITEM) )
    {
        //Call the main function
        if ( Align(ATE::kLeftJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(ALIGN_CENTER_MENU_ITEM) )
    {
        //Call the main function
        if ( Align(ATE::kCenterJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(ALIGN_RIGHT_MENU_ITEM) )
    {
        //Call the main function
        if ( Align(ATE::kRightJustify) ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Align"), ai::UnicodeString("Redo Align"));
        }
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(CREATE_MICR_BARCODE_MENU_ITEM) )
    {
        //Call the main function
        if ( CreateMICRBarcode() ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Create MICR Barcode"), ai::UnicodeString("Redo Create MICR Barcode"));
        }
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(LIST_FONTS_MENU_ITEM) )
    {
        //Call the main function
        ListFonts listFonts;
        if ( listFonts.PutFontList() ) {
            //Set the undo/redo text
            error = sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Create Fonts List"), ai::UnicodeString("Redo Create Fonts List"));
        }
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(PRINT_TO_PDF_MENU_ITEM) )
    {
        printToPdfUIController->LoadExtension();
        sAICSXSExtension->LaunchExtension(PrintToPdf::PrintToPdfUIController::PRINTTOPDF_UI_EXTENSION);
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(CREATE_PLATE_BLEED_INFO_MENU_ITEM) )
    {
        ai::UnicodeString menuText;
        sAIMenu->GetItemText(message->menuItem, menuText);
        if (menuText.find(ai::UnicodeString("Remove"),0) != ai::UnicodeString::npos)
        {
            sgJobFile->RemoveBleedInfo();
            sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Remove Safeguard Plate Info"), ai::UnicodeString("Redo Remove Safeguard Plate Info"));
        }
        else
        {
            sgJobFile->Update();
            sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Add Safeguard Plate Info"), ai::UnicodeString("Redo Add Safeguard Plate Info"));
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
	
	if (message->menuItem == menuItemHandles.GetHandleWithKey(FIND_AND_REPLACE_MENU_ITEM) )
    {
        AIBoolean state;
        sAICSXSExtension->IsPrimaryStageVisible(ColorToolsUIController::COLORTOOLS_UI_EXTENSION, state);
		if (state == TRUE)
		{
			sAIMenu->SetItemText( message->menuItem, ai::UnicodeString("Hide Find and Replace Graphics") );
		}
		else
		{
			sAIMenu->SetItemText( message->menuItem, ai::UnicodeString("Show Find and Replace Graphics") );
		}
	}
    
    if (message->menuItem == menuItemHandles.GetHandleWithKey(CREATE_MICR_BARCODE_MENU_ITEM) )
    {
        //Check if we have a micr line object in the document dictionary
        //If we do, nothing needs to be selected, as we already know where the micr line is
        if ( CheckDictionaryForArtObjectWithIdentifier(ai::UnicodeString(MICR_LINE_LABEL)) )
        {
            sAIMenu->EnableItem(message->menuItem);
        }
        else
        {
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
    
    if (message->menuItem == menuItemHandles.GetHandleWithKey(CREATE_PLATE_BLEED_INFO_MENU_ITEM) )
    {
        //Check if we have a bleed info in the dictionary
        //If we do, change to "Remove"
        if ( CheckDictionaryForArtObjectWithIdentifier(ai::UnicodeString(SafeguardFile::PLATE_BLEED_INFO_GROUP_LABEL), 0) )
        {
            sAIMenu->SetItemText( message->menuItem, ai::UnicodeString("Remove Safeguard Plate Info") );
        }
        else
        {
            sAIMenu->SetItemText( message->menuItem, ai::UnicodeString("Add Safeguard Plate Info") );
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
        printToPdfUIController->RegisterCSXSEventListeners();
    }
    
    if ( message->notifier == fAppStartedNotifierHandle )
    {
        // Whatever we want to do when the app starts
    }
    
    if (message->notifier == fDocOpenedNotifierHandle ||
        message->notifier == fCustomColorChangeNotifierHandle ||
        message->notifier == fSwatchLibChangeNotifierHandle )
    {
        string swatchesXml = gPlugin->GetBtSwatchList()->GetColorListAsXMLString();
        colorToolsUIController->SendColorListXmlToHtml(swatchesXml);
    }
    
    if (message->notifier == fArtSelectionChangeNotifierHandle )
    {
        colorToolsUIController->DetermineChangeInStatus();
    }
    if (message->notifier == fDocumentCropAreaModifiedNotifierHandle )
    {
        if (sgJobFile)
        {
            sgJobFile->Update();
        }
    }
    return kNoErr;
}

#include "IllustratorSDK.h"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"

#include "AICSXS.h"
#include "AIArtboardMessage.h"

#include "GetIllustratorErrorCode.h"

#include "ColorToolsUIController.h"
#include "PrintToPdfUIController.h"
#include "BtSwatchList.h"
#include "BtAiMenuItem.h"
#include "BtAiMenuItemHandles.h"
#include "FixBlack.h"

#include "TextTools.h"
#include "FixFreehandType.h"
#include "DictionaryWriter.h"
#include "PdfPrinter.h"
#include "BleedInfo.h"
#include "ListFonts.h"
#include "BtDocumentView.hpp"

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
    fAppStartedNotifierHandle(NULL),
    fDocOpenedNotifierHandle(NULL),
    fDocumentCropAreaModifiedNotifierHandle(NULL),
    fArtSelectionChangedNotifierHandle(NULL)
{
	strncpy(fPluginName, kSafeguardToolsPluginName, kMaxStringLength);
}

SafeguardToolsPlugin::~SafeguardToolsPlugin()
{
}

ASErr SafeguardToolsPlugin::Message(char* caller, char* selector, void *message)
{
	ASErr error = kNoErr;
    
/*    if (strcmp(caller, kCallerAIPluginGroup) == 0)
    {
        if (strcmp( selector, kSelectorAINotifyEdits ) == 0)
        {
            error = BleedInfo::PluginGroupNotify((AIPluginGroupMessage *)message);
        }
        else if (strcmp( selector, kSelectorAIUpdateArt ) == 0)
        {
            error = BleedInfo::PluginGroupUpdate((AIPluginGroupMessage *)message);
        }
    }
    else
    {
 */       try
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
//    }
    
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
    
    error = this->AddMenus(message);
    if (error) { return error; }
    
    //Register PlateBleedInfo plugin group
    error = sAIPluginGroup->AddAIPluginGroup (message->d.self,
                                      CREATE_PLATE_BLEED_INFO_PLUGIN_GROUP,
                                      &pluginGroupData,
                                      kPluginGroupKeepWhenEmptyOption | kPluginGroupDoNotTarget | kPluginGroupDoNotSmartTarget | kPluginGroupAskToShowContents,
                                      &bleedInfoPluginGroupHandle);
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
                                     kAIDocumentCropAreaModifiedNotifier, &fDocumentCropAreaModifiedNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAIArtSelectionChangedNotifier, &fArtSelectionChangedNotifierHandle);
    if (error) { return error; }
    
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
    if (plateBleedInfoUIController)
    {
        plateBleedInfoUIController->RemoveEventListeners();
        Plugin::LockPlugin(false);
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
    ASErr error = kNoErr;
    
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
    
    if (NULL == plateBleedInfoUIController)
    {
        plateBleedInfoUIController = std::make_shared<SafeguardFile::PlateBleedInfoUIController>();
        
        error = Plugin::LockPlugin(true);
        if (error) { return error; }
    }

    
    return error;
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
    BtAiMenuItem ModifySwatchesMenu = BtAiMenuItem(menuItem, kMenuGroupSortedAlphabeticallyOption);
    
    menuItem.groupName = modifySwatchesGroup;
    menuItem.itemText = ai::UnicodeString(FIX_BLACK_MENU_ITEM);
    BtAiMenuItem FixBlackMenuItem = BtAiMenuItem(menuItem, kMenuItemNoOptions);
    ModifySwatchesMenu.AddSubMenuItem(FixBlackMenuItem);
    
    menuItem.groupName = modifySwatchesGroup;
    menuItem.itemText = ai::UnicodeString(FIND_AND_REPLACE_MENU_ITEM);
    BtAiMenuItem FindAndReplaceGraphicsMenuItem = BtAiMenuItem(menuItem, kMenuItemWantsUpdateOption);
    ModifySwatchesMenu.AddSubMenuItem(FindAndReplaceGraphicsMenuItem);
    
    BtAiMenuItem::AddMenu(ModifySwatchesMenu, &menuItemHandles);
    
	
    //TEXT TOOLS MENU
    const char* textToolsGroup = TEXT_TOOLS_MENU;
    
    menuItem.groupName = kTypeLayoutMenuGroup;
    menuItem.itemText = ai::UnicodeString(TEXT_TOOLS_MENU);
    BtAiMenuItem TextToolsMenu = BtAiMenuItem(menuItem, kMenuGroupSortedAlphabeticallyOption);
    
/*    menuItem.groupName = textToolsGroup;
    menuItem.itemText = ai::UnicodeString(MAKE_POINT_TYPE_MENU_ITEM);
    BtAiMenuItem MakePointTypeMenuItem = BtAiMenuItem(menuItem, kMenuItemNoOptions);
    MakePointTypeMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfText, 0, 0, 0);
    TextToolsMenu.AddSubMenuItem(MakePointTypeMenuItem);
*/
    menuItem.groupName = textToolsGroup;
    menuItem.itemText = ai::UnicodeString(FIX_FREEHAND_TYPE_MENU_ITEM);
    BtAiMenuItem FixFreehandTypeMenuItem = BtAiMenuItem(menuItem, kMenuItemNoOptions);
    FixFreehandTypeMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfText, 0, 0, 0);
    TextToolsMenu.AddSubMenuItem(FixFreehandTypeMenuItem);
    
    BtAiMenuItem::AddMenu(TextToolsMenu, &menuItemHandles);
    
    
    //ALIGN MENU
    const char* alignGroup = ALIGN_MENU;
    
    menuItem.groupName = kObjectAttribsMenuGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_MENU);
    BtAiMenuItem AlignMenu = BtAiMenuItem(menuItem, kMenuGroupSortedAlphabeticallyOption);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_LEFT_MENU_ITEM);
    BtAiMenuItem AlignLeftMenuItem = BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    AlignLeftMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu.AddSubMenuItem(AlignLeftMenuItem);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_CENTER_MENU_ITEM);
    BtAiMenuItem AlignCenterMenuItem = BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    AlignCenterMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu.AddSubMenuItem(AlignCenterMenuItem);
    
    menuItem.groupName = alignGroup;
    menuItem.itemText = ai::UnicodeString(ALIGN_RIGHT_MENU_ITEM);
    BtAiMenuItem AlignRightMenuItem = BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    AlignRightMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0);
    AlignMenu.AddSubMenuItem(AlignRightMenuItem);
    
    BtAiMenuItem::AddMenu(AlignMenu, &menuItemHandles);

    
    //CREATE MICR BARCODE
    menuItem.groupName = kTypePluginsMenuGroup1;
    menuItem.itemText = ai::UnicodeString(CREATE_MICR_BARCODE_MENU_ITEM);
    BtAiMenuItem CreateMicrBarcodeMenuItem = BtAiMenuItem(menuItem, kMenuItemIgnoreSort|kMenuItemWantsUpdateOption);
    
    BtAiMenuItem::AddMenu(CreateMicrBarcodeMenuItem, &menuItemHandles);
    
    
    //List Fonts
    menuItem.groupName = kTypePluginsMenuGroup1;
    menuItem.itemText = ai::UnicodeString(LIST_FONTS_MENU_ITEM);
    BtAiMenuItem ListFontsMenuItem = BtAiMenuItem(menuItem, kMenuItemIgnoreSort);
    ListFontsMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    
    BtAiMenuItem::AddMenu(ListFontsMenuItem, &menuItemHandles);
    

    //PRINT TO PDF
    menuItem.groupName = kSaveForMenuGroup;
    menuItem.itemText = ai::UnicodeString(PRINT_TO_PDF_MENU_ITEM);
    BtAiMenuItem PrintToPdfMenuItem = BtAiMenuItem(menuItem, kMenuItemNoOptions);
    PrintToPdfMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    
    BtAiMenuItem::AddMenu(PrintToPdfMenuItem, &menuItemHandles);

    
    //CREATE SLUG INFO
    menuItem.groupName = kDocumentUtilsMenuGroup;
    menuItem.itemText = ai::UnicodeString(CREATE_PLATE_BLEED_INFO_MENU_ITEM);
    BtAiMenuItem CreatePlateBleedInfoMenuItem = BtAiMenuItem(menuItem, kMenuItemWantsUpdateOption);
    CreatePlateBleedInfoMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    
    BtAiMenuItem::AddMenu(CreatePlateBleedInfoMenuItem, &menuItemHandles);

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
/*  else if ( message->menuItem == menuItemHandles.GetHandleWithKey(MAKE_POINT_TYPE_MENU_ITEM) )
    {
        //Call the main function
        if ( ConvertToPointType() ) {
            //What to do if it worked.
        }
    }
*/  else if ( message->menuItem == menuItemHandles.GetHandleWithKey(FIX_FREEHAND_TYPE_MENU_ITEM) )
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
        if ( IsBleedInfoPluginArtCreated() )
        {
            SafeguardJobFile sgJobFile;
            sgJobFile.EditBleedInfo();
            sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Edit Safeguard Plate Info"), ai::UnicodeString("Redo Edit Safeguard Plate Info"));
        }
        else
        {
            SafeguardJobFile sgJobFile;
            sgJobFile.UpdateBleedInfo();
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
        DictionaryWriter dw;
        if (dw.CheckDictionaryForArtObjectWithIdentifier(MICR_LINE_LABEL) )
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
        if ( IsBleedInfoPluginArtCreated() )
        {
            sAIMenu->SetItemText( message->menuItem, ai::UnicodeString("Edit Safeguard Plate Info") );
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

ASErr SafeguardToolsPlugin::PluginGroupUpdate(AIPluginGroupMessage* message)
{
    SafeguardJobFile sgJobFile;
    sgJobFile.UpdateBleedInfo();
    
    return kNoErr;
}

ASErr SafeguardToolsPlugin::PluginGroupNotify(AIPluginGroupMessage* message)
{
    if (strcmp( message->code, kAttachOperationCode ) == 0 && strcmp( message->time, kCheckOperationTime ) == 0)
    {
        return kNoErr;
    }
    if (strcmp( message->code, kAttachOperationCode ) == 0 && strcmp( message->time, kBeforeOperationTime ) == 0)
    {
        return kNoErr;
    }
    if (strcmp( message->code, kAttachOperationCode ) == 0 && strcmp( message->time, kAfterOperationTime ) == 0)
    {
        return kMarkValidPluginGroupReply;
    }
    return kUnhandledMsgErr;
}

ASErr SafeguardToolsPlugin::Notify(AINotifierMessage *message )
{
    if ( message->notifier == fRegisterEventNotifierHandle)
    {
        colorToolsUIController->RegisterCSXSEventListeners();
        printToPdfUIController->RegisterCSXSEventListeners();
        plateBleedInfoUIController->RegisterCSXSEventListeners();
    }
    
    if ( message->notifier == fAppStartedNotifierHandle )
    {
        // Whatever we want to do when the app starts
    }
    
    if (message->notifier == fDocOpenedNotifierHandle)
    {
        BtSwatchList swatchList;
        string swatchesXml = swatchList.GetColorListAsXMLString();
        colorToolsUIController->SendColorListXmlToHtml(swatchesXml);
    }
    
    if (message->notifier == fArtSelectionChangedNotifierHandle )
    {
        colorToolsUIController->UpdateChangeInStatus();
    }
    if (message->notifier == fDocumentCropAreaModifiedNotifierHandle || message->notifier == fArtSelectionChangedNotifierHandle)
    {
        if ( IsBleedInfoPluginArtCreated() )
        {
            size_t gTimeStamp = sAIArt->GetGlobalTimeStamp();
            DictionaryWriter dw;
            AIReal aTSDict = dw.GetAIRealFromIdentifier(SafeguardFile::PLATE_BLEEDINFO_TIMESTAMP);
            
            if ( gTimeStamp != aTSDict )
            {
                sAINotifier->SetNotifierActive(fDocumentCropAreaModifiedNotifierHandle, false);
                try
                {
                    SafeguardJobFile sgJobFile;
                    sgJobFile.UpdateBleedInfo();
                }
                catch (std::runtime_error e)
                {
                    e.what(); //BALLS!
                }
                sAINotifier->SetNotifierActive(fDocumentCropAreaModifiedNotifierHandle, true);
            }
        }
    }
    return kNoErr;
}

bool SafeguardToolsPlugin::IsBleedInfoPluginArtCreated()
{
    DictionaryWriter dw;
    if ( dw.CheckDictionaryForIdentifier(SafeguardFile::SG_BLEEDINFO_ARTHANDLES) )
    {
        return true;
    }
    return false;
}

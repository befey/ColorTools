#include "IllustratorSDK.h"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"

#include "AICSXS.h"
#include "AIArtboardMessage.h"

#include "GetIllustratorErrorCode.h"

#include "BtSwatchList.h"
#include "BtAiMenuItem.h"
#include "BtAiMenuItemHandles.h"
#include "FixBlack.h"

#include "TextTools.h"
#include "FixFreehandType.h"
#include "DictionaryWriter.h"
#include "PdfPrinter.h"
#include "ListFonts.h"
#include "BtDocumentView.hpp"
#include "BleedInfoController.hpp"

SafeguardToolsPlugin *gPlugin = nullptr;

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
    fRegisterEventNotifierHandle(nullptr),
    fAppStartedNotifierHandle(nullptr),
    fDocOpenedNotifierHandle(nullptr),
    fDocumentCropAreaModifiedNotifierHandle(nullptr),
    fArtSelectionChangedNotifierHandle(nullptr),
    bleedInfoPluginGroupHandle(nullptr)
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
    error = sAINotifier->AddNotifier( fPluginRef, kSafeguardToolsPluginName,
                                     kAIDocumentSavedNotifier, &fDocumentSavedNotifierHandle);
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
    if (printToPdfFolderPrefsUIController)
    {
        printToPdfFolderPrefsUIController->RemoveEventListeners();
        Plugin::LockPlugin(false);
    }
    if (plateBleedInfoUIController)
    {
        plateBleedInfoUIController->RemoveEventListeners();
        Plugin::LockPlugin(false);
    }
    if (plateBleedInfoUIController)
    {
        placeFileSearchUIController->RemoveEventListeners();
        Plugin::LockPlugin(false);
    }

    message->d.globals = nullptr;
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
    
    if (nullptr == colorToolsUIController)
    {
        colorToolsUIController = std::make_shared<ColorToolsUIController>();
        
        error = Plugin::LockPlugin(true);
        if (error) { return error; }
    }
    
    if (nullptr == printToPdfUIController)
    {
        printToPdfUIController = std::make_shared<PrintToPdf::PrintToPdfUIController>();
        
        error = Plugin::LockPlugin(true);
        if (error) { return error; }
    }
    
    if (nullptr == printToPdfFolderPrefsUIController)
    {
        printToPdfFolderPrefsUIController = std::make_shared<PrintToPdf::PrintToPdfFolderPrefsUIController>();
        
        error = Plugin::LockPlugin(true);
        if (error) { return error; }
    }
    
    if (nullptr == plateBleedInfoUIController)
    {
        plateBleedInfoUIController = std::make_shared<PlateBleedInfo::PlateBleedInfoUIController>();
        
        error = Plugin::LockPlugin(true);
        if (error) { return error; }
    }
    
    if (nullptr == placeFileSearchUIController)
    {
        placeFileSearchUIController = std::make_shared<PlaceFileSearch::PlaceFileSearchUIController>();
        
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
    BtAiMenuItem ModifySwatchesMenu(kEditMenuGroup, MODIFY_SWATCHES_MENU, kMenuGroupSortedAlphabeticallyOption);
    ModifySwatchesMenu.AddSubMenuItem( BtAiMenuItem(FIX_BLACK_MENU_ITEM, kMenuItemNoOptions) );
    ModifySwatchesMenu.AddSubMenuItem( BtAiMenuItem(FIND_AND_REPLACE_MENU_ITEM, kMenuItemWantsUpdateOption) );
    BtAiMenuItem::AddMenu(ModifySwatchesMenu, &menuItemHandles);
	
    
    //TEXT TOOLS MENU
    BtAiMenuItem TextToolsMenu = BtAiMenuItem(kTypeLayoutMenuGroup, TEXT_TOOLS_MENU, kMenuGroupSortedAlphabeticallyOption);
    TextToolsMenu.AddSubMenuItem(
                                 BtAiMenuItem(FIX_FREEHAND_TYPE_MENU_ITEM, kMenuItemNoOptions)
                                 .SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfText, 0, 0, 0)
                                 );
    BtAiMenuItem::AddMenu(TextToolsMenu, &menuItemHandles);
    
    
    //ALIGN MENU
    BtAiMenuItem AlignMenu = BtAiMenuItem(kObjectAttribsMenuGroup, ALIGN_MENU, kMenuGroupSortedAlphabeticallyOption);
    AlignMenu.AddSubMenuItem(
                             BtAiMenuItem(ALIGN_LEFT_MENU_ITEM, kMenuItemIgnoreSort)
                             .SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0)
                             );
    AlignMenu.AddSubMenuItem(
                             BtAiMenuItem(ALIGN_CENTER_MENU_ITEM, kMenuItemIgnoreSort)
                             .SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0)
                             );
    AlignMenu.AddSubMenuItem(
                             BtAiMenuItem(ALIGN_RIGHT_MENU_ITEM, kMenuItemIgnoreSort)
                             .SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, kIfAnyArt, 0, 0, 0)
                             );
    BtAiMenuItem::AddMenu(AlignMenu, &menuItemHandles);

    
    //CREATE MICR BARCODE
    BtAiMenuItem CreateMicrBarcodeMenuItem = BtAiMenuItem(kTypePluginsMenuGroup1, CREATE_MICR_BARCODE_MENU_ITEM, kMenuItemIgnoreSort|kMenuItemWantsUpdateOption);
    BtAiMenuItem::AddMenu(CreateMicrBarcodeMenuItem, &menuItemHandles);
    
    
    //List Fonts
    BtAiMenuItem ListFontsMenuItem = BtAiMenuItem(kTypePluginsMenuGroup1, LIST_FONTS_MENU_ITEM, kMenuItemIgnoreSort);
    ListFontsMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    BtAiMenuItem::AddMenu(ListFontsMenuItem, &menuItemHandles);
    

    //PRINT TO PDF
    BtAiMenuItem PrintToPdfMenuItem = BtAiMenuItem(kSaveForMenuGroup, PRINT_TO_PDF_MENU_ITEM, kMenuItemNoOptions);
    PrintToPdfMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    BtAiMenuItem::AddMenu(PrintToPdfMenuItem, &menuItemHandles);

    
    //CREATE SLUG INFO
    BtAiMenuItem CreatePlateBleedInfoMenuItem = BtAiMenuItem(kDocumentUtilsMenuGroup, CREATE_PLATE_BLEED_INFO_MENU_ITEM, kMenuItemWantsUpdateOption);
    CreatePlateBleedInfoMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    BtAiMenuItem::AddMenu(CreatePlateBleedInfoMenuItem, &menuItemHandles);
    
    BtAiMenuItem EditPlateBleedInfoMenuItem = BtAiMenuItem(kDocumentUtilsMenuGroup, EDIT_PLATE_BLEED_INFO_MENU_ITEM, kMenuItemWantsUpdateOption);
    EditPlateBleedInfoMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    BtAiMenuItem::AddMenu(EditPlateBleedInfoMenuItem, &menuItemHandles);

    
    //Place File Search
    BtAiMenuItem SafeguardMfgPlaceMenuItem = BtAiMenuItem(kPlaceMenuGroup, SG_MFG_PLACE_MENU_ITEM, kMenuItemNoOptions);
    SafeguardMfgPlaceMenuItem.SetAutoUpdateOptions(kAutoEnableMenuItemAction, 0, 0, 0, 0, kIfOpenDocument, 0);
    BtAiMenuItem::AddMenu(SafeguardMfgPlaceMenuItem, &menuItemHandles);
    
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
        FixBlack(); //We call this twice to ensure Black and White get renamed correctly
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
        PlateBleedInfo::BleedInfoController({fDocumentCropAreaModifiedNotifierHandle,fArtSelectionChangedNotifierHandle}).HandleCreateMenu();
    }
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(EDIT_PLATE_BLEED_INFO_MENU_ITEM) )
    {
        PlateBleedInfo::BleedInfoController({fDocumentCropAreaModifiedNotifierHandle,fArtSelectionChangedNotifierHandle}).HandleEditMenu();
    }
    
    else if ( message->menuItem == menuItemHandles.GetHandleWithKey(SG_MFG_PLACE_MENU_ITEM) )
    {
        placeFileSearchUIController->LoadExtension();
        sAICSXSExtension->LaunchExtension(PlaceFileSearch::PlaceFileSearchUIController::PLACEFILESEARCH_UI_EXTENSION);
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

	if (error)
		goto error;

error:
	return error;
}

ASErr SafeguardToolsPlugin::PluginGroupUpdate(AIPluginGroupMessage* message)
{
    if (message->art != nullptr)
    {
        PlateBleedInfo::BleedInfoController({fDocumentCropAreaModifiedNotifierHandle,fArtSelectionChangedNotifierHandle}).HandlePluginGroupUpdate(message);
    }
    
    return kNoErr;
}

ASErr SafeguardToolsPlugin::PluginGroupNotify(AIPluginGroupMessage* message)
{
    if (message->entry == bleedInfoPluginGroupHandle)
    {
        return PlateBleedInfo::BleedInfoController().HandlePluginGroupNotify(message);
    }
    return kUnhandledMsgErr;
}

ASErr SafeguardToolsPlugin::Notify(AINotifierMessage *message )
{
    if ( message->notifier == fRegisterEventNotifierHandle)
    {
        colorToolsUIController->RegisterCSXSEventListeners();
        printToPdfUIController->RegisterCSXSEventListeners();
        printToPdfFolderPrefsUIController->RegisterCSXSEventListeners();
        plateBleedInfoUIController->RegisterCSXSEventListeners();
        placeFileSearchUIController->RegisterCSXSEventListeners();
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
        
        PlateBleedInfo::BleedInfoController biController({fDocumentCropAreaModifiedNotifierHandle,fArtSelectionChangedNotifierHandle});
        biController.RedrawOnDocOpen();
    }
    
    if (message->notifier == fArtSelectionChangedNotifierHandle )
    {
        colorToolsUIController->UpdateChangeInStatus();
        
        PlateBleedInfo::BleedInfoController biController({fDocumentCropAreaModifiedNotifierHandle,fArtSelectionChangedNotifierHandle});
        biController.DeSelectAllPluginArts();
        biController.HandleSelectionChangeNotification();
    }
    if (message->notifier == fDocumentCropAreaModifiedNotifierHandle)
    {
        PlateBleedInfo::BleedInfoController biController({fDocumentCropAreaModifiedNotifierHandle,fArtSelectionChangedNotifierHandle});
        biController.DeSelectAllPluginArts();
        biController.HandleCropAreaNotification();
    }
    if (message->notifier == fDocumentSavedNotifierHandle)
    {
        PlateBleedInfo::BleedInfoController biController({fDocumentCropAreaModifiedNotifierHandle,fArtSelectionChangedNotifierHandle});
        biController.DeSelectAllPluginArts();
        biController.RedrawOnDocOpen();
    }
    return kNoErr;
}

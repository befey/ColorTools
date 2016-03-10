#include "IllustratorSDK.h"
#include "ColorToolsPlugin.h"
#include "ColorToolsSuites.h"

#include "AICSXS.h"

#include "ColorToolsUIController.h"
#include "BtSwatchList.h"
#include "FixBlack.h"

ColorToolsPlugin *gPlugin = NULL;

Plugin* AllocatePlugin(SPPluginRef pluginRef)
{
	return new ColorToolsPlugin(pluginRef);
}

void FixupReload(Plugin* plugin)
{
	ColorToolsPlugin::FixupVTable((ColorToolsPlugin*) plugin);
}

ColorToolsPlugin::ColorToolsPlugin(SPPluginRef pluginRef) :
	Plugin(pluginRef),
    fRegisterEventNotifierHandle(NULL),
    colorToolsUIController(NULL),
    mySwatchList(NULL)
{
	strncpy(fPluginName, kColorToolsPluginName, kMaxStringLength);
}

ColorToolsPlugin::~ColorToolsPlugin()
{
}

ASErr ColorToolsPlugin::Message(char* caller, char* selector, void *message)
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

ASErr ColorToolsPlugin::SetGlobal(Plugin *plugin)
{
    gPlugin = (ColorToolsPlugin *) plugin;
    return kNoErr;
}


ASErr ColorToolsPlugin::StartupPlugin( SPInterfaceMessage *message )
{
	ASErr error = kNoErr;
    error = Plugin::StartupPlugin(message);
    if (error) { return error; }
    
    error = this->AddMenus(message);
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
    
    //Register for notifiers
    error = sAINotifier->AddNotifier( fPluginRef, kColorToolsPluginName,
                                     kAICSXSPlugPlugSetupCompleteNotifier, &fRegisterEventNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kColorToolsPluginName,
                                     kAIApplicationStartedNotifier, &fAppStartedNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kColorToolsPluginName,
                                     kAIDocumentOpenedNotifier, &fDocOpenedNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kColorToolsPluginName,
                                     kAIArtCustomColorChangedNotifier, &fCustomColorChangeNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kColorToolsPluginName,
                                     kAISwatchLibraryChangedNotifier, &fSwatchLibChangeNotifierHandle);
    if (error) { return error; }
    error = sAINotifier->AddNotifier( fPluginRef, kColorToolsPluginName,
                                     kAIArtSelectionChangedNotifier, &fArtSelectionChangeNotifierHandle);
    if (error) { return error; }
    
    return error;
}

ASErr ColorToolsPlugin::ShutdownPlugin( SPInterfaceMessage *message )
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

ASErr ColorToolsPlugin::ReloadPlugin(SPInterfaceMessage *message)
{
    return Plugin::ReloadPlugin(message);
}

ASErr ColorToolsPlugin::UnloadPlugin(SPInterfaceMessage *message)
{
    return Plugin::UnloadPlugin(message);
}

/*
 */
ASErr ColorToolsPlugin::PostStartupPlugin()
{
    return kNoErr;
}

/*
 */
ASErr ColorToolsPlugin::AddMenus(SPInterfaceMessage* message)
{
	AIErr error = kNoErr;
	
	// Add the menus.
	AIPlatformAddMenuItemDataUS menuData;
	AIPlatformAddMenuItemDataUS fixBlackMenuData;
	AIPlatformAddMenuItemDataUS addColorMenuData;
	AIPlatformAddMenuItemDataUS findReplaceMenuData;
	
	const char *menuGroupCStr = "SDKEditGroup";
	const char *menuCStr = "Modify Swatches";
	const char *fixBlackCStr = "Fix Black";
	const char *findReplaceCStr = "Find and Replace Graphics";
	AIMenuGroup	menuGroup;
	AIMenuGroup	pluginMenuGroup;
	AIMenuGroup dummyGroup;
	AIMenuItemHandle dummyItem;
	bool sdkGroupAlreadyMade = false;
	bool findReplaceGroupAlreadyMade = false;
	
	//This line tells AI to put the new item in the Edit menu
	menuData.groupName = kEditMenuGroup;
	menuData.itemText = ai::UnicodeString(menuCStr);
	
	fixBlackMenuData.groupName = menuGroupCStr;
	fixBlackMenuData.itemText = ai::UnicodeString(fixBlackCStr);
	
	findReplaceMenuData.groupName = menuGroupCStr;
	findReplaceMenuData.itemText = ai::UnicodeString(findReplaceCStr);
    
	
	// search through the menus and
	// see if another SDK plug in has already set up the group
	int count;
	error = sAIMenu->CountMenuGroups( &count );
	if ( error ) goto error;
	while (count > 0)
	{
		error = sAIMenu->GetNthMenuGroup( count-1, &dummyGroup );
		if ( error ) goto error;
		char *name;
		error = sAIMenu->GetMenuGroupName( dummyGroup, (const char**)&name );
		if ( error ) goto error;
		if ( std::strcmp(name, menuGroupCStr ) == 0 )
		{
			sdkGroupAlreadyMade = true;
			count = -1;
		}
		if(std::strcmp(name,findReplaceCStr) == 0)
		{
			findReplaceGroupAlreadyMade = true;
		}
		count--;
	}
	
	if ( !sdkGroupAlreadyMade )
	{
		error = sAIMenu->AddMenuItem( message->d.self, menuGroupCStr, &menuData, 0, &dummyItem );
		if ( error ) goto error;
		
		error = sAIMenu->AddMenuGroupAsSubMenu( menuGroupCStr, kMenuGroupSortedAlphabeticallyOption, dummyItem, &menuGroup );
		if ( error ) goto error;
	}

	if(!findReplaceGroupAlreadyMade)
	{
		error = sAIMenu->AddMenuItem( message->d.self, findReplaceCStr, &findReplaceMenuData, 0, &dummyItem );
		if ( error ) goto error;
		
		error = sAIMenu->AddMenuGroupAsSubMenu( findReplaceCStr, 0, dummyItem, &pluginMenuGroup );
		if ( error ) goto error;
	}
	
	
	error = sAIMenu->AddMenuItem( message->d.self, fixBlackCStr, &fixBlackMenuData, kMenuItemNoOptions, &FixBlackMenuItemSelected );
	if ( error ) goto error;
	
	findReplaceMenuData.groupName = findReplaceCStr;
	error = sAIMenu->AddMenuItem( message->d.self, findReplaceCStr, &findReplaceMenuData, kMenuItemWantsUpdateOption, &FindReplaceMenuItemSelected );
	if ( error ) goto error;
	
error:
	return error;
}

/*
 */
ASErr ColorToolsPlugin::GoMenuItem(AIMenuMessage* message)
{
	AIErr error = kNoErr;
	
	if ( message->menuItem == FixBlackMenuItemSelected )
	{
        FixBlack();
    }
	if ( message->menuItem == FindReplaceMenuItemSelected )
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
	
	if (error)
		goto error;
	
error:
	return error;
}


ASErr ColorToolsPlugin::UpdateMenuItem(AIMenuMessage* message)
{
	AIErr error = kNoErr;
	
	if (message->menuItem == FindReplaceMenuItemSelected) {
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
	
	if (error)
		goto error;
	
error:
	return error;
}

ASErr ColorToolsPlugin::Notify(AINotifierMessage *message )
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

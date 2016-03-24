#ifndef __SafeguardToolsPlugin_h__
#define __SafeguardToolsPlugin_h__

#include "Plugin.hpp"
#include "AIMenuGroups.h"
#include "SDKDef.h"
#include "SDKAboutPluginsHelper.h"
#include "SafeguardToolsID.h"

class ColorToolsUIController;
class BtSwatchList;

/**	Creates a new SafeguardToolsPlugin.
	@param pluginRef IN unique reference to this plugin.
	@return pointer to new SafeguardToolsPlugin.
*/
Plugin* AllocatePlugin(SPPluginRef pluginRef);

/**	Reloads the SafeguardToolsPlugin class state when the plugin is
	reloaded by the application.
	@param plugin IN pointer to plugin being reloaded.
*/
void FixupReload(Plugin* plugin);

/**	Hooks SafeguardTools up as an Illustrator plug-in.
	@ingroup SafeguardTools
*/
class SafeguardToolsPlugin : public Plugin
{
public:
	/** Constructor.
		@param pluginRef IN reference to this plugin.
	*/
	SafeguardToolsPlugin(SPPluginRef pluginRef);

	/** Destructor.
	*/
	virtual ~SafeguardToolsPlugin();
    
    
    ColorToolsUIController* GetColorToolsUIController() const { return colorToolsUIController; }
    BtSwatchList* GetBtSwatchList() const { return mySwatchList; }
    void SetBtSwatchList(BtSwatchList *swatchList) { mySwatchList = swatchList; }
    
  
    /**	Restores state of SafeguardToolsPlugin during reload.
	*/
	FIXUP_VTABLE_EX(SafeguardToolsPlugin, Plugin);
    
protected:
	/** Calls Plugin::Message and handles any errors returned.
     @param caller IN sender of the message.
     @param selector IN nature of the message.
     @param message IN pointer to plugin and call information.
     @return kNoErr on success, other ASErr otherwise.
     */
	virtual ASErr Message(char* caller, char* selector, void *message);
    
    virtual ASErr SetGlobal(Plugin* plugin);
  
  /**	Initializes the plugin.
		@param message IN message sent by the plugin manager.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr StartupPlugin( SPInterfaceMessage * message );

	/**	Removes the plugin.
		@param message IN message sent by the plugin manager.
		@return kNoErr on success, other ASErr otherwise.
	*/
	ASErr ShutdownPlugin( SPInterfaceMessage * message );
    
    /** Performs plugin tasks that could not be performed until
     the application was started.
     @return kNoErr on success, other ASErr otherwise.
     */
    virtual ASErr PostStartupPlugin();
    
    virtual ASErr GoMenuItem(AIMenuMessage* message);
    
	/**	Updates state of menu items.
     @param message IN pointer to plugin and call information.
     @return kNoErr on success, other ASErr otherwise.
     */
	virtual ASErr UpdateMenuItem(AIMenuMessage* message);
    
    virtual ASErr ReloadPlugin(SPInterfaceMessage *message);
    
    virtual ASErr UnloadPlugin(SPInterfaceMessage *message);
    
    virtual ASErr Notify(AINotifierMessage* message);
private:
    BtSwatchList *mySwatchList;
    ColorToolsUIController *colorToolsUIController;
    
    AINotifierHandle fRegisterEventNotifierHandle;
    AINotifierHandle fAppStartedNotifierHandle;
    AINotifierHandle fDocOpenedNotifierHandle;
    AINotifierHandle fCustomColorChangeNotifierHandle;
    AINotifierHandle fSwatchLibChangeNotifierHandle;
    AINotifierHandle fArtSelectionChangeNotifierHandle;
    
	/**	Menu item handles**/
    AIMenuItemHandle FixBlackMenuItemSelected;
    AIMenuItemHandle FindReplaceMenuItemSelected;
     
	/**	Adds the menu items for this plugin to the application UI.
     @param message IN pointer to plugin and call information.
     @return kNoErr on success, other ASErr otherwise.
     */
	ASErr AddMenus(SPInterfaceMessage* message);

};

#endif

#ifndef __SafeguardToolsPlugin_h__
#define __SafeguardToolsPlugin_h__

//=================================
// included dependencies
#include "Plugin.hpp"
#include "AIMenuGroups.h"
#include "SDKDef.h"
#include "SafeguardToolsID.h"
#include "BtAiMenuItemHandles.h"
#include "PrintToPdfUIController.h"
#include "SafeguardJobFile.h"

//=================================
// forward declared dependencies
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
    
    
    shared_ptr<BtSwatchList> GetBtSwatchList() const { return mySwatchList; }
    AIPluginGroupHandle GetCreatePlateBleedInfoPluginGroupHandle() const { return createPlateBleedInfoPluginGroupHandle; }
    
    unique_ptr<SafeguardJobFile> sgJobFile;
  
    /**	Restores state of SafeguardToolsPlugin during reload.
	*/
	FIXUP_VTABLE_EX(SafeguardToolsPlugin, Plugin);
    
        
    //=================================
    // Constant definitions
    static constexpr auto MODIFY_SWATCHES_MENU =           "Modify Swatches";
    static constexpr auto FIX_BLACK_MENU_ITEM =            "Fix Black";
    static constexpr auto FIND_AND_REPLACE_MENU_ITEM =     "Find and Replace Graphics";
    
    static constexpr auto TEXT_TOOLS_MENU =                "Text Tools";
    static constexpr auto MAKE_POINT_TYPE_MENU_ITEM =      "Make Point Type";
    static constexpr auto FIX_FREEHAND_TYPE_MENU_ITEM =    "Fix Freehand Type";
    
    static constexpr auto ALIGN_MENU =                     "Align";
    static constexpr auto ALIGN_LEFT_MENU_ITEM =           "Left";
    static constexpr auto ALIGN_CENTER_MENU_ITEM =         "Center";
    static constexpr auto ALIGN_RIGHT_MENU_ITEM =          "Right";
    
    static constexpr auto CREATE_MICR_BARCODE_MENU_ITEM =  "Create MICR Barcode";
    static constexpr auto LIST_FONTS_MENU_ITEM =           "Generate Font List";
    static constexpr auto PRINT_TO_PDF_MENU_ITEM =         "Print to PDF...";
    
    static constexpr auto CREATE_PLATE_BLEED_INFO_PLUGIN_GROUP =  "bt.SafeguardTools.PlateBleedInfo";
    static constexpr auto CREATE_PLATE_BLEED_INFO_MENU_ITEM =     "Create Slug Info";

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
    shared_ptr<BtSwatchList> mySwatchList;
    shared_ptr<ColorToolsUIController> colorToolsUIController;
    shared_ptr<PrintToPdf::PrintToPdfUIController> printToPdfUIController;
    
    /**	Notifier handles**/
    AINotifierHandle fRegisterEventNotifierHandle;
    AINotifierHandle fAppStartedNotifierHandle;
    AINotifierHandle fDocOpenedNotifierHandle;
    AINotifierHandle fCustomColorChangeNotifierHandle;
    AINotifierHandle fSwatchLibChangeNotifierHandle;
    AINotifierHandle fArtSelectionChangeNotifierHandle;
    AINotifierHandle fDocumentCropAreaModifiedNotifierHandle;
    
	/**	Menu item handles**/
    BtAiMenuItemHandles menuItemHandles;
    
    /** Plugin Group handle **/
    AIPluginGroupHandle createPlateBleedInfoPluginGroupHandle;
     
	/**	Adds the menu items for this plugin to the application UI.
     @param message IN pointer to plugin and call information.
     @return kNoErr on success, other ASErr otherwise.
     */
	ASErr AddMenus(SPInterfaceMessage* message);

};

#endif

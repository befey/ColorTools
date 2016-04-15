//
//  ColorToolsUIController.h
//  SafeguardTools
//
//  Created by T431962 on 5/19/15.
//
//

#ifndef __SafeguardTools__ColorToolsUIController__
#define __SafeguardTools__ColorToolsUIController__

#include "SDKPlugPlug.h"
#include "FlashUIController.h"


#define COLORTOOLS_UI_EXTENSION             "com.gosafeguard.SafeguardTools.ColorTools"
#define EVENT_TYPE_CHANGE_CLICKED           "com.gosafeguard.SafeguardTools.ColorTools.changebutton"
#define EVENT_TYPE_REMOVE_CLICKED           "com.gosafeguard.SafeguardTools.ColorTools.removebutton"
#define EVENT_TYPE_UPDATE_COLOR_LIST        "com.gosafeguard.SafeguardTools.ColorTools.updatelist"
#define EVENT_TYPE_UPDATE_COLOR_LIST_BACK   "com.gosafeguard.SafeguardTools.ColorTools.updatelistback"
#define EVENT_TYPE_CHANGE_COUNT_BACK        "com.gosafeguard.SafeguardTools.ColorTools.changecountback"
#define EVENT_TYPE_CHANGE_IN_BACK           "com.gosafeguard.SafeguardTools.ColorTools.changeinback"
#define EVENT_TYPE_FORCE_PANEL_CLOSE        "com.gosafeguard.SafeguardTools.ColorTools.forcepanelclose"
#define ILST_APP                            "ILST"


class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;

static void ChangeButtonClickedFunc (const csxs::event::Event* const event, void* const context);
static void RemoveButtonClickedFunc (const csxs::event::Event* const event, void* const context);

class ColorToolsUIController : public FlashUIController
{
public:
    ColorToolsUIController(void);
    
    /** Registers the events this plug-in will listen for with PlugPlug.
     @return error code if error found, or success otherwise.
     */
    csxs::event::EventErrorCode RegisterCSXSEventListeners();
    
    /** Removes the previously added event listeners from PlugPlug.
     @return error code if error found, or success otherwise.
     */
    csxs::event::EventErrorCode RemoveEventListeners();
    
    ASErr SendData();
    ASErr SendColorListXmlToHtml(string swatchesXml);
    ASErr SendChangeCountToHtml(int count);
    
    void DetermineChangeInStatus();
    ASErr SendChangeInToHtml(int changeIn);
    void SendCloseMessageToHtml();
    
    void ParseData(const char* eventData);
    
    static void ChangeButtonClickedFunc (const csxs::event::Event* const event, void* const context);
    static void RemoveButtonClickedFunc (const csxs::event::Event* const event, void* const context);
    static void UpdateListFunc (const csxs::event::Event* const event, void* const context);

private:
};

#endif /* defined(__SafeguardTools__ColorToolsUIController__) */

//
//  ColorToolsUIController.h
//  SafeguardTools
//
//  Created by T431962 on 5/19/15.
//
//

#ifndef __SafeguardTools__ColorToolsUIController__
#define __SafeguardTools__ColorToolsUIController__

//=================================
// included dependencies
#include "SDKPlugPlug.h"
#include "FlashUIController.h"

//=================================
// forward declared dependencies
class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;

class ColorToolsUIController : public FlashUIController
{
public:
    //=================================
    // Constant definitions
    static constexpr auto COLORTOOLS_UI_EXTENSION =           "com.gosafeguard.SafeguardTools.ColorTools";
    static constexpr auto EVENT_TYPE_CHANGE_CLICKED =         "com.gosafeguard.SafeguardTools.ColorTools.changebutton";
    static constexpr auto EVENT_TYPE_REMOVE_CLICKED =         "com.gosafeguard.SafeguardTools.ColorTools.removebutton";
    static constexpr auto EVENT_TYPE_UPDATE_COLOR_LIST =      "com.gosafeguard.SafeguardTools.ColorTools.updatelist";
    static constexpr auto EVENT_TYPE_UPDATE_COLOR_LIST_BACK = "com.gosafeguard.SafeguardTools.ColorTools.updatelistback";
    static constexpr auto EVENT_TYPE_CHANGE_COUNT_BACK =      "com.gosafeguard.SafeguardTools.ColorTools.changecountback";
    static constexpr auto EVENT_TYPE_CHANGE_IN_BACK =         "com.gosafeguard.SafeguardTools.ColorTools.changeinback";
    static constexpr auto EVENT_TYPE_FORCE_PANEL_CLOSE =      "com.gosafeguard.SafeguardTools.ColorTools.forcepanelclose";
    static constexpr auto ILST_APP =                          "ILST";
    
    static constexpr auto ATTRIBUTE_SELECT =    "attribute-select";
    enum class Attribute
    {
        Color =                                 0,
        Overprint =                             1
    };
    
    static constexpr auto CHANGEIN_SELECT =     "changein-select";
    enum class ChangeIn
    {
        Selection =                             0,
        Document =                              1
    };
    
    static constexpr auto ADDREMOVE_SELECT =    "addremove-select";
    enum class AddRemove
    {
        Add =                                   0,
        Remove =                                1
    };
    
    static constexpr auto APPLYTO_SELECT =      "applyto-select";
    enum class ApplyTo
    {
        FillsAndStrokes =                       0,
        Fills =                                 1,
        Strokes =                               2
    };
    
    static constexpr auto FROM_SELECT =         "from-select";
    static constexpr auto TO_SELECT =           "to-select";
    static constexpr auto TINTS_CHECKBOX =      "tints-checkbox";

    
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
    
    void UpdateChangeInStatus();
    ASErr SendChangeInToHtml(ChangeIn changeIn);
    void SendCloseMessageToHtml();
    
    void ParseData(const char* eventData);
    
    static void ChangeButtonClickedFunc (const csxs::event::Event* const event, void* const context);
    static void RemoveButtonClickedFunc (const csxs::event::Event* const event, void* const context);
    static void UpdateListFunc (const csxs::event::Event* const event, void* const context);

private:

};

#endif /* defined(__SafeguardTools__ColorToolsUIController__) */

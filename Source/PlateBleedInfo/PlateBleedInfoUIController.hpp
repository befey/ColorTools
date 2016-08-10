//
//  PlateBleedInfoUIController.hpp
//  SafeguardTools
//
//  Created by T431962 on 8/3/16.
//
//

#ifndef PlateBleedInfoUIController_hpp
#define PlateBleedInfoUIController_hpp

//=================================
// included dependencies
#include <string>
#include "SDKPlugPlug.h"
#include "FlashUIController.h"
#include "SafeguardJobFile.h"

//=================================
// forward declared dependencies
class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;

namespace SafeguardFile
{
    //=================================
    // PlateBleedInfoUIController - handles communication between extension and plugin
    class PlateBleedInfoUIController : public FlashUIController
    {
    public:
        PlateBleedInfoUIController(void);
        
        /** Registers the events this plug-in will listen for with PlugPlug.
         @return error code if error found, or success otherwise.
         */
        csxs::event::EventErrorCode RegisterCSXSEventListeners();
        
        /** Removes the previously added event listeners from PlugPlug.
         @return error code if error found, or success otherwise.
         */
        csxs::event::EventErrorCode RemoveEventListeners();
        
        ASErr SendData();
        
        void ParseData(const char* eventData);
        
        static void PanelLoaded (const csxs::event::Event* const event, void* const context);
        static void CancelButtonClickedFunc (const csxs::event::Event* const event, void* const context);
        static void ChangeArtboardFunc (const csxs::event::Event* const event, void* const context);
        void SendCloseMessageToHtml();
        void SendBleedInfoToHtml();

        
        //=================================
        // Constant definitions
        static constexpr auto PLATEBLEEDINFO_UI_EXTENSION =     "com.gosafeguard.SafeguardTools.PlateBleedInfo";
        static constexpr auto PLATEBLEEDINFO_PANEL_LOADED =     "com.gosafeguard.SafeguardTools.PlateBleedInfo.panelloaded";
        static constexpr auto PLATEBLEEDINFO_DATA_FROM_PLUGIN = "com.gosafeguard.SafeguardTools.PlateBleedInfo.datafromplugin";
        static constexpr auto PLATEBLEEDINFO_CHANGE_ARTBOARD =  "com.gosafeguard.SafeguardTools.PlateBleedInfo.changeartboard";
        static constexpr auto EVENT_TYPE_CANCEL_CLICKED =       "com.gosafeguard.SafeguardTools.PlateBleedInfo.cancelbutton";
        static constexpr auto EVENT_TYPE_FORCE_PANEL_CLOSE =    "com.gosafeguard.SafeguardTools.PlateBleedInfo.forcepanelclose";
        
        static constexpr auto PLATEBLEEDINFO_ARTBOARD_ID =      "artboard-id";
        
        static constexpr auto ILST_APP =                        "ILST";
        
    private:
        static ai::ArtboardID GetArtboardIdFromJson(const char* json);
        string GetBleedInfoAsJson() const;

    };
}

#endif /* PlateBleedInfoUIController_hpp */

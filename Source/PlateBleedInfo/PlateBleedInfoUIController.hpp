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
        
        void EditBleedInfo(SafeguardFile::SafeguardJobFile sgJobFile);
        
        //=================================
        // Constant definitions
        static constexpr auto PLATEBLEEDINFO_UI_EXTENSION =     "com.gosafeguard.SafeguardTools.PlateBleedInfo";
        static constexpr auto ILST_APP =                        "ILST";
        
    private:
        
    };
}

#endif /* PlateBleedInfoUIController_hpp */

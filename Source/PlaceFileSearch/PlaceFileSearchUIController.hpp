//
//  PlaceFileSearchUIController.hpp
//  SafeguardTools
//
//  Created by T431962 on 1/12/17.
//
//

#ifndef PlaceFileSearchUIController_hpp
#define PlaceFileSearchUIController_hpp

//=================================
// included dependencies
#include "SDKPlugPlug.h"
#include "FlashUIController.h"
#include "AIDocument.h"
#include "AIUser.h"

//=================================
// forward declared dependencies
class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;

extern AIDocumentSuite* sAIDocument;
extern AIUserSuite* sAIUser;

namespace PlaceFileSearch
{
    class PlaceFileSearchUIController : public FlashUIController
    {
    public:
        PlaceFileSearchUIController(void);
        
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
        
        static void SearchButtonClickedFunc (const csxs::event::Event* const event, void* const context);
        static void PlaceFileEventFunc (const csxs::event::Event* const event, void* const context);
        static void CancelButtonClickedFunc (const csxs::event::Event* const event, void* const context);
        
        ASErr SendResultsXmlToHtml(string resultsXml);
        void SendCloseMessageToHtml();
        
        //=================================
        // Constant definitions
        static constexpr auto PLACEFILESEARCH_UI_EXTENSION =    "com.gosafeguard.SafeguardTools.PlaceFileSearch";
        static constexpr auto EVENT_TYPE_PLACE_FILE =           "com.gosafeguard.SafeguardTools.PlaceFileSearch.placefile";
        static constexpr auto EVENT_TYPE_SEARCH_CLICKED =       "com.gosafeguard.SafeguardTools.PlaceFileSearch.searchbutton";
        static constexpr auto EVENT_TYPE_CANCEL_CLICKED =       "com.gosafeguard.SafeguardTools.PlaceFileSearch.cancelbutton";
        static constexpr auto EVENT_TYPE_RESULTS_BACK =         "com.gosafeguard.SafeguardTools.PlaceFileSearch.resultsback";
        static constexpr auto EVENT_TYPE_FORCE_PANEL_CLOSE =    "com.gosafeguard.SafeguardTools.PlaceFileSearch.forcepanelclose";
        static constexpr auto ILST_APP =                        "ILST";
                
    private:
        
    };
}



#endif /* PlaceFileSearchUIController_hpp */

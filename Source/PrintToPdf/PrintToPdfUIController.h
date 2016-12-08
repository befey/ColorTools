//
//  PrintToPdfUIController.h
//  SafeguardTools
//
//  Created by T431962 on 4/15/16.
//
//

#ifndef __SafeguardTools__PrintToPdfUIController__
#define __SafeguardTools__PrintToPdfUIController__

//=================================
// included dependencies
#include <string>
#include "SDKPlugPlug.h"
#include "FlashUIController.h"
#include "AIDocument.h"

//=================================
// forward declared dependencies
class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;

extern AIDocumentSuite* sAIDocument;

namespace PrintToPdf
{
    //=================================
    // PrintToPdfUIController - handles communication between extension and plugin
    class PrintToPdfUIController : public FlashUIController
    {
    public:
        PrintToPdfUIController(void);
        
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
        static void MakePdfButtonClickedFunc (const csxs::event::Event* const event, void* const context);
        static void CancelButtonClickedFunc (const csxs::event::Event* const event, void* const context);
        static void NoArtboardsSelectedHandler (const csxs::event::Event* const event, void* const context);
        
        ASErr SendResultsXmlToHtml(string resultsXml);
        void SendCloseMessageToHtml();
        void ClearResultsBox();
        void SendColorListToHtml();
        string GetJsonBleedInfoDto() const;
        
        //=================================
        // Constant definitions
        static constexpr auto PRINTTOPDF_UI_EXTENSION =         "com.gosafeguard.SafeguardTools.PrintToPdf";
        static constexpr auto EVENT_TYPE_MAKEPDF_CLICKED =      "com.gosafeguard.SafeguardTools.PrintToPdf.makepdfbutton";
        static constexpr auto EVENT_TYPE_CANCEL_CLICKED =       "com.gosafeguard.SafeguardTools.PrintToPdf.cancelbutton";
        static constexpr auto EVENT_TYPE_RESULTS_BACK =         "com.gosafeguard.SafeguardTools.PrintToPdf.resultsback";
        static constexpr auto EVENT_TYPE_FORCE_PANEL_CLOSE =    "com.gosafeguard.SafeguardTools.PrintToPdf.forcepanelclose";
        static constexpr auto EVENT_TYPE_PANEL_LOADED =         "com.gosafeguard.SafeguardTools.PrintToPdf.panelloaded";
        static constexpr auto EVENT_TYPE_DATA_FROM_PLUGIN =     "com.gosafeguard.SafeguardTools.PrintToPdf.datafromplugin";
        static constexpr auto EVENT_TYPE_CLEAR_RESULT_BOX =     "com.gosafeguard.SafeguardTools.PrintToPdf.clearresultbox";
        static constexpr auto EVENT_TYPE_NO_ARTBOARDS_SEL =     "com.gosafeguard.SafeguardTools.PrintToPdf.noartboardsselected";
        static constexpr auto ILST_APP =                        "ILST";
        
        static constexpr auto PRESET_SELECT =                   "preset-select";
                
        static constexpr auto DTO =                             "dto";
        static constexpr auto SEPARATEFILES_CHECK =             "separatefiles-check";
        static constexpr auto USEROUTPUTFOLDER_CHECK =          "useroutputfolder-check";
        static constexpr auto DONOTDELETE_CHECK =               "donotdelete-check";
        static constexpr auto RESULTS_TEXTAREA =                "results-textarea";
        
    private:
        
    };
}

#endif /* defined(__SafeguardTools__PrintToPdfUIController__) */

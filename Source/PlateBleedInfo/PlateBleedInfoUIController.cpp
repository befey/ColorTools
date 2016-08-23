//
//  PlateBleedInfoUIController.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/3/16.
//
//

#include "AppContext.hpp"
#include "PlateBleedInfoUIController.hpp"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"
#include "BtDocumentView.hpp"
#include "GetIllustratorErrorCode.h"
#include "PlateBleedInfoDTO.hpp"
#include "rapidjson/document.h"
#include "cereal/archives/json.hpp"

using SafeguardFile::PlateBleedInfoUIController;
using SafeguardFile::SafeguardJobFile;

void PlateBleedInfoUIController::PanelLoaded (const csxs::event::Event* const event, void* const context)
{
    PlateBleedInfoUIController *plateBleedInfoUIController = (PlateBleedInfoUIController *)context;
    if (NULL == plateBleedInfoUIController || event == NULL)
    {
        return;
    }
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        plateBleedInfoUIController->SendBleedInfoToHtml();
        
        // Clean up the application context and return.
    } while(false);
    return;
}

void PlateBleedInfoUIController::OkButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    PlateBleedInfoUIController *plateBleedInfoUIController = (PlateBleedInfoUIController *)context;
    if (NULL == plateBleedInfoUIController || event == NULL)
    {
        return;
    }
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        SafeguardFile::PlateBleedInfoDTO plateBleedInfoDTO;
        std::stringstream ss;
        ss << event->data;
        string s = ss.str();
        {
            cereal::JSONInputArchive iarchive(ss); // Create an input archive
            try {
            iarchive(plateBleedInfoDTO);
            }
            catch(cereal::Exception const& e) {
                string es = e.what();
                int foo = 0;
            }
        }
        
        BtDocumentView docView;
        docView.RecallDocumentView();
        
        plateBleedInfoUIController->SendCloseMessageToHtml();
        
        // Clean up the application context and return.
    } while(false);
    return;
}

void PlateBleedInfoUIController::CancelButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    PlateBleedInfoUIController *plateBleedInfoUIController = (PlateBleedInfoUIController *)context;
    if (NULL == plateBleedInfoUIController || event == NULL)
    {
        return;
    }
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        BtDocumentView docView;
        docView.RecallDocumentView();
        
        plateBleedInfoUIController->SendCloseMessageToHtml();
        
        // Clean up the application context and return.
    } while(false);
    return;
}

void PlateBleedInfoUIController::ChangeArtboardFunc (const csxs::event::Event* const event, void* const context)
{
    PlateBleedInfoUIController *plateBleedInfoUIController = (PlateBleedInfoUIController *)context;
    if (NULL == plateBleedInfoUIController || event == NULL)
    {
        return;
    }
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        //TODO: Center on the new artboard
        BtDocumentView docview;
        docview.SetViewOnArtboard( GetArtboardIdFromJson(event->data) );
        
        sAIDocument->RedrawDocument();
        sAIDocument->RedrawDocument();
        // Clean up the application context and return.
    } while(false);
    return;
}


PlateBleedInfoUIController::PlateBleedInfoUIController(void)
: FlashUIController(PLATEBLEEDINFO_UI_EXTENSION)
{
}


/* Add event listeners
 */
csxs::event::EventErrorCode PlateBleedInfoUIController::RegisterCSXSEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.AddEventListener(PLATEBLEEDINFO_PANEL_LOADED, PanelLoaded, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(PLATEBLEEDINFO_DATA_FROM_EXT, OkButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(EVENT_TYPE_CANCEL_CLICKED, CancelButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(PLATEBLEEDINFO_CHANGE_ARTBOARD, ChangeArtboardFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
    }
    while(false);
    return result;
}

/* Remove event listeners
 */
csxs::event::EventErrorCode PlateBleedInfoUIController::RemoveEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.RemoveEventListener(PLATEBLEEDINFO_PANEL_LOADED, PanelLoaded, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(PLATEBLEEDINFO_DATA_FROM_EXT, OkButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_CANCEL_CLICKED, CancelButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(PLATEBLEEDINFO_CHANGE_ARTBOARD, ChangeArtboardFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
    }
    while(false);
    return result;
}

ASErr PlateBleedInfoUIController::SendData()
{
    AIErr error = kNoErr;
    return error;
}

/* Parse received data from flash panel.
 */
void PlateBleedInfoUIController::ParseData(const char* eventData)
{
    return;
}

void PlateBleedInfoUIController::SendCloseMessageToHtml()
{
    csxs::event::Event event = {
        EVENT_TYPE_FORCE_PANEL_CLOSE,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        NULL
    };
    fPPLib.DispatchEvent(&event);
}

void PlateBleedInfoUIController::SendBleedInfoToHtml()
{
    string json = GetBleedInfoAsJson();
    csxs::event::Event event = {
        PLATEBLEEDINFO_DATA_FROM_PLUGIN,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        json.c_str()
    };
    fPPLib.DispatchEvent(&event);
}

ai::ArtboardID PlateBleedInfoUIController::GetArtboardIdFromJson(const char* json)
{
    using namespace rapidjson;
    
    Document d;
    d.Parse(json);
    
    Value& v = d[PlateBleedInfoUIController::PLATEBLEEDINFO_ARTBOARD_ID];
    
    return ai::ArtboardID(v.GetInt());
}

string PlateBleedInfoUIController::GetBleedInfoAsJson() const
{
    std::stringstream ss;
    {
        SafeguardFile::PlateBleedInfoDTO dto;
        cereal::JSONOutputArchive oarchive(ss); // Create an output archive
        oarchive(CEREAL_NVP(dto));
    }
    return ss.str();
}
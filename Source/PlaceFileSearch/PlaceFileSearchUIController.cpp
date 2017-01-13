//
//  PlaceFileSearchUIController.cpp
//  SafeguardTools
//
//  Created by T431962 on 1/12/17.
//
//

#include "AppContext.hpp"
#include "PlaceFileSearchUIController.hpp"
#include "SafeguardToolsPlugin.h"
#include "PlaceFileSearcher.hpp"
#include "FilePlacer.hpp"

using PlaceFileSearch::PlaceFileSearchUIController;
using PlaceFileSearch::FilePlacer;

void PlaceFileSearchUIController::SearchButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    PlaceFileSearchUIController *placeFileSearchUIController = (PlaceFileSearchUIController *)context;
    if(NULL == placeFileSearchUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        PlaceFileSearch::PlaceFileSearcher searcher(string(event->data));
        string resultsXML = searcher.DoSearch().GetResultsAsXML();
        placeFileSearchUIController->SendResultsXmlToHtml(resultsXML);
        
    } while(false);
    return;
}

void PlaceFileSearchUIController::PlaceFileEventFunc (const csxs::event::Event* const event, void* const context)
{
    PlaceFileSearchUIController *placeFileSearchUIController = (PlaceFileSearchUIController *)context;
    if(NULL == placeFileSearchUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        //placeFileSearchUIController->SendCloseMessageToHtml();
        FilePlacer(string(event->data)).Place();
        sAIDocument->RedrawDocument();
        sAIUser->AppIdle();
        
    } while(false);
    return;
}

void PlaceFileSearchUIController::CancelButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    PlaceFileSearchUIController *placeFileSearchUIController = (PlaceFileSearchUIController *)context;
    if(NULL == placeFileSearchUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        placeFileSearchUIController->SendCloseMessageToHtml();
        
        // Clean up the application context and return.
    } while(false);
    return;
}

PlaceFileSearchUIController::PlaceFileSearchUIController(void)
: FlashUIController(PLACEFILESEARCH_UI_EXTENSION)
{
}


/* Add event listeners
 */
csxs::event::EventErrorCode PlaceFileSearchUIController::RegisterCSXSEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.AddEventListener(EVENT_TYPE_SEARCH_CLICKED, SearchButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(EVENT_TYPE_PLACE_FILE, PlaceFileEventFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(EVENT_TYPE_CANCEL_CLICKED, CancelButtonClickedFunc, this);
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
csxs::event::EventErrorCode PlaceFileSearchUIController::RemoveEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_SEARCH_CLICKED, SearchButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_PLACE_FILE, PlaceFileEventFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_CANCEL_CLICKED, CancelButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
    }
    while(false);
    return result;
}

ASErr PlaceFileSearchUIController::SendData()
{
    AIErr error = kNoErr;
    return error;
}

/* Parse received data from flash panel.
 */
void PlaceFileSearchUIController::ParseData(const char* eventData)
{
    return;
}

ASErr PlaceFileSearchUIController::SendResultsXmlToHtml(string resultsXml)
{
    AIErr error = kNoErr;
    
    csxs::event::Event event = {
        EVENT_TYPE_RESULTS_BACK,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        resultsXml.c_str()
    };
    fPPLib.DispatchEvent(&event);
    
    return error;
}

void PlaceFileSearchUIController::SendCloseMessageToHtml()
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

//
//  PrintToPdfUIController.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/15/16.
//
//

#include "AppContext.hpp"
#include "PrintToPdfUIController.h"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"
#include "PdfPrinter.h"
#include "PdfSettings.h"

using PrintToPdf::PrintToPdfUIController;

void PrintToPdfUIController::MakePdfButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    PrintToPdfUIController *printToPdfUIController = (PrintToPdfUIController *)context;
    if(NULL == printToPdfUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        printToPdfUIController->ClearResultsBox();
        
        PdfSettings settings = PdfSettings::MakePdfSettingsFromXml(event->data);
        string results = PdfPrinter::GetPrinter(settings.OutputSeparateFiles())->Print(settings).MakeXmlString();
        printToPdfUIController->SendResultsXmlToHtml(results);
        
        sAIUndo->UndoChanges();
    } while(false);
    return;
}

void PrintToPdfUIController::CancelButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    PrintToPdfUIController *printToPdfUIController = (PrintToPdfUIController *)context;
    if(NULL == printToPdfUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        printToPdfUIController->SendCloseMessageToHtml();
        
        // Clean up the application context and return.
    } while(false);
    return;
}


PrintToPdfUIController::PrintToPdfUIController(void)
: FlashUIController(PRINTTOPDF_UI_EXTENSION)
{
}


/* Add event listeners
 */
csxs::event::EventErrorCode PrintToPdfUIController::RegisterCSXSEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.AddEventListener(EVENT_TYPE_MAKEPDF_CLICKED, MakePdfButtonClickedFunc, this);
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
csxs::event::EventErrorCode PrintToPdfUIController::RemoveEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_MAKEPDF_CLICKED, MakePdfButtonClickedFunc, this);
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

ASErr PrintToPdfUIController::SendData()
{
    AIErr error = kNoErr;
    return error;
}

/* Parse received data from flash panel.
 */
void PrintToPdfUIController::ParseData(const char* eventData)
{
    return;
}

ASErr PrintToPdfUIController::SendResultsXmlToHtml(string resultsXml)
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

void PrintToPdfUIController::SendCloseMessageToHtml()
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

void PrintToPdfUIController::ClearResultsBox()
{
    csxs::event::Event event = {
        EVENT_TYPE_CLEAR_RESULT_BOX,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        NULL
    };
    fPPLib.DispatchEvent(&event);
}
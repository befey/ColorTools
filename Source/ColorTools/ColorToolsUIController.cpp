//
//  ColorToolsUIController.cpp
//  SafeguardTools
//
//  Created by T431962 on 5/19/15.
//
//

#include "AppContext.hpp"
#include "ColorToolsUIController.h"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"
#include "BtSwatchList.h"
#include "ReplaceData.h"
#include "FindReplaceGraphics.h"
#include <stdio.h>
#include <memory.h>
#include "ColorFuncs.h"

void ColorToolsUIController::ChangeButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    ColorToolsUIController *colorToolsUIController = (ColorToolsUIController *)context;
    if(NULL == colorToolsUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        //Set the undo/redo text
        sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Change Colors"), ai::UnicodeString("Redo Change Colors"));
        
        unique_ptr<ReplaceData> data = make_unique<ReplaceData>(event->data);
        
        colorToolsUIController->SendChangeCountToHtml(FindAndReplace(data));
        
    } while(false);
    return;
}

void ColorToolsUIController::RemoveButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    ColorToolsUIController *colorToolsUIController = (ColorToolsUIController *)context;
    if(NULL == colorToolsUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        //Set the undo/redo text
        sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Remove Unused Colors"), ai::UnicodeString("Redo Remove Unused Colors"));
        
        BtSwatchList swatchList;
        swatchList.RemoveUnusedColors();
        UpdateListFunc(event, context);
        // Clean up the application context and return.
    } while(false);
    return;
}

void ColorToolsUIController::UpdateListFunc (const csxs::event::Event* const event, void* const context)
{
    ColorToolsUIController *colorToolsUIController = (ColorToolsUIController *)context;
    if(NULL == colorToolsUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        sAIUndo->SetSilent(TRUE);
        BtSwatchList swatchList;
        string swatchesXml = swatchList.GetColorListAsXMLString();
        
        colorToolsUIController->SendColorListXmlToHtml(swatchesXml);
        
        // Clean up the application context and return.
    } while(false);
    return;
}

ColorToolsUIController::ColorToolsUIController(void)
: FlashUIController(COLORTOOLS_UI_EXTENSION)
{
}


/* Add event listeners
 */
csxs::event::EventErrorCode ColorToolsUIController::RegisterCSXSEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.AddEventListener(EVENT_TYPE_CHANGE_CLICKED, ChangeButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(EVENT_TYPE_REMOVE_CLICKED, RemoveButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(EVENT_TYPE_UPDATE_COLOR_LIST, UpdateListFunc, this);
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
csxs::event::EventErrorCode ColorToolsUIController::RemoveEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_CHANGE_CLICKED, ChangeButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_REMOVE_CLICKED, RemoveButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_UPDATE_COLOR_LIST, UpdateListFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
     }
    while(false);
    return result;
}

ASErr ColorToolsUIController::SendData()
{
    AIErr error = kNoErr;
    return error;
}

/* Parse received data from flash panel.
 */
void ColorToolsUIController::ParseData(const char* eventData)
{
    return;
}

ASErr ColorToolsUIController::SendColorListXmlToHtml(string swatchesXml)
{
    AIErr error = kNoErr;

    csxs::event::Event event = {
        EVENT_TYPE_UPDATE_COLOR_LIST_BACK,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        swatchesXml.c_str()
    };
    fPPLib.DispatchEvent(&event);
    
    return error;
}

ASErr ColorToolsUIController::SendChangeCountToHtml(int count)
{
    AIErr error = kNoErr;
    
    char buffer[10];
    sprintf(buffer, "%d", count);
    
    csxs::event::Event event = {
        EVENT_TYPE_CHANGE_COUNT_BACK,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        buffer
    };
    fPPLib.DispatchEvent(&event);
    
    return error;
}

void ColorToolsUIController::UpdateChangeInStatus()
{
    AIArtHandle** matches;
    int count = 0;
    
    sAIDocumentList->Count(&count);
    if (count) {
        sAIMatchingArt->GetSelectedArt(&matches, &count);
        
        if (count) {
            SendChangeInToHtml(ChangeIn::Selection);
        } else {
            SendChangeInToHtml(ChangeIn::Document);
        }
        
        sAIMdMemory->MdMemoryDisposeHandle((void**)matches);
    }
}

ASErr ColorToolsUIController::SendChangeInToHtml(ChangeIn changeIn)
{
    AIErr error = kNoErr;
    
    char buffer[10];
    sprintf(buffer, "%d", changeIn);
    
    csxs::event::Event event = {
        EVENT_TYPE_CHANGE_IN_BACK,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        buffer
    };
    fPPLib.DispatchEvent(&event);
    
    return error;
}

void ColorToolsUIController::SendCloseMessageToHtml()
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

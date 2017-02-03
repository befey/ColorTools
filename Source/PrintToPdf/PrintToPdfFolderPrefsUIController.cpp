//
//  PrintToPdfFolderPrefsUIController.cpp
//  SafeguardTools
//
//  Created by T431962 on 2/1/17.
//
//

#include "AppContext.hpp"
#include "PrintToPdfFolderPrefsUIController.hpp"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"
#include "PreferenceWriter.hpp"
#include "PrintToPdfConstants.h"

using PrintToPdf::PrintToPdfFolderPrefsUIController;

void PrintToPdfFolderPrefsUIController::PanelLoaded (const csxs::event::Event* const event, void* const context)
{
    PrintToPdfFolderPrefsUIController *printToPdfFolderPrefsUIController = (PrintToPdfFolderPrefsUIController *)context;
    if (NULL == printToPdfFolderPrefsUIController || event == NULL)
    {
        return;
    }
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        printToPdfFolderPrefsUIController->SendFolderPrefsToHtml();
        
        // Clean up the application context and return.
    } while(false);
    return;
}

void PrintToPdfFolderPrefsUIController::OkButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    PrintToPdfFolderPrefsUIController *printToPdfFolderPrefsUIController = (PrintToPdfFolderPrefsUIController *)context;
    if(NULL == printToPdfFolderPrefsUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        
        
        } while(false);
    return;
}

void PrintToPdfFolderPrefsUIController::CancelButtonClickedFunc (const csxs::event::Event* const event, void* const context)
{
    PrintToPdfFolderPrefsUIController *printToPdfFolderPrefsUIController = (PrintToPdfFolderPrefsUIController *)context;
    if(NULL == printToPdfFolderPrefsUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());
        
        printToPdfFolderPrefsUIController->SendCloseMessageToHtml();
        
        // Clean up the application context and return.
    } while(false);
    return;
}

PrintToPdfFolderPrefsUIController::PrintToPdfFolderPrefsUIController(void)
: FlashUIController(PRINTTOPDF_FOLDERPREFS_EXTENSION)
{
}


/* Add event listeners
 */
csxs::event::EventErrorCode PrintToPdfFolderPrefsUIController::RegisterCSXSEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.AddEventListener(EVENT_TYPE_OK_CLICKED, OkButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(EVENT_TYPE_CANCEL_CLICKED, CancelButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.AddEventListener(EVENT_TYPE_PANEL_LOADED, PanelLoaded, this);
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
csxs::event::EventErrorCode PrintToPdfFolderPrefsUIController::RemoveEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_OK_CLICKED, OkButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_CANCEL_CLICKED, CancelButtonClickedFunc, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_PANEL_LOADED, PanelLoaded, this);
        if (result != csxs::event::kEventErrorCode_Success)
        {
            break;
        }
    }
    while(false);
    return result;
}

ASErr PrintToPdfFolderPrefsUIController::SendData()
{
    AIErr error = kNoErr;
    return error;
}

/* Parse received data from flash panel.
 */
void PrintToPdfFolderPrefsUIController::ParseData(const char* eventData)
{
    return;
}

ASErr PrintToPdfFolderPrefsUIController::SendFolderPrefsToHtml()
{
    AIErr error = kNoErr;
    string folders = GetPrintToPdfFolderPrefsAsXml();
    
    csxs::event::Event event = {
        EVENT_TYPE_RESULTS_BACK,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        folders.c_str()
    };
    fPPLib.DispatchEvent(&event);
    
    return error;
}

string PrintToPdfFolderPrefsUIController::GetPrintToPdfFolderPrefsAsXml()
{
    //Format as XML string
    string xmlString;
    
    PreferenceWriter writer(PRINTTOPDF_FOLDERPREFS_EXTENSION);
    vector<std::pair<string, ai::FilePath>> folders;
    ai::FilePath foundPath;
    
    if ( writer.GetFilePathFromIdentifier(MANUFACTURING_PDF_PRESET, foundPath) )
    {
        folders.push_back({MANUFACTURING_PDF_PRESET, foundPath});
    }
    else
    {
        folders.push_back({MANUFACTURING_PDF_PRESET, ai::FilePath(ai::UnicodeString(PATH_TO_PLANT_MANUFACTURING))});
    }
    
    if ( writer.GetFilePathFromIdentifier(REG_PROOF_PDF_PRESET, foundPath) )
    {
        folders.push_back({REG_PROOF_PDF_PRESET, foundPath});
    }
    else
    {
        folders.push_back({REG_PROOF_PDF_PRESET, ai::FilePath(ai::UnicodeString(PATH_TO_PDFPROOFS))});
    }

    if ( writer.GetFilePathFromIdentifier(MICR_PROOF_PDF_PRESET, foundPath) )
    {
        folders.push_back({MICR_PROOF_PDF_PRESET, foundPath});
    }
    else
    {
        folders.push_back({MICR_PROOF_PDF_PRESET, ai::FilePath(ai::UnicodeString(PATH_TO_PDFPROOFS))});
    }
    
    xmlString.append("<root>");
    for (auto it : folders)
    {
        xmlString.append("<folder>");
        xmlString.append("<preset-name>").append(it.first).append("</preset-name>");
        xmlString.append("<path>").append(it.second.GetFullPath().getInStdString(kAIPlatformCharacterEncoding)).append("</path>");
        xmlString.append("</folder>");
    }
    xmlString.append("</root>");
    
    return xmlString;
}

void PrintToPdfFolderPrefsUIController::SendCloseMessageToHtml()
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

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

#include "cereal/external/rapidjson/document.h"

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
        
        vector<std::pair<string, ai::FilePath>> folders;
        
        using namespace rapidjson;
        
        Document d;
        d.Parse(event->data);
        
        for ( auto& folder : d.GetArray() )
        {
            auto f = folder.GetArray();
            folders.push_back({f[0].GetString(), ai::FilePath(ai::UnicodeString(f[1].GetString()))});
        }
        
        printToPdfFolderPrefsUIController->StoreFoldersInPrefs(folders);
        
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

void PrintToPdfFolderPrefsUIController::ChangeFolderFunc (const csxs::event::Event* const event, void* const context)
{
    PrintToPdfFolderPrefsUIController *printToPdfFolderPrefsUIController = (PrintToPdfFolderPrefsUIController *)context;
    if(NULL == printToPdfFolderPrefsUIController || event == NULL)
        return;
    
    do {
        // Set up the application context, so that suite calls can work.
        AppContext appContext(gPlugin->GetPluginRef());

        using namespace rapidjson;
        
        Document d;
        d.Parse(event->data);

        Value& v = d[PrintToPdfFolderPrefsUIController::CHG_FOLDER_PRESET_NAME];
        string presetname = (v.GetString());
        
        v = d[PrintToPdfFolderPrefsUIController::CHG_FOLDER_PATH];
        string path = (v.GetString());
        
        ai::FilePath aiPath = ChooseNewFolder(path);
        
        printToPdfFolderPrefsUIController->SendNewPathToDialog(presetname, aiPath);
        
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
        result =  fPPLib.AddEventListener(EVENT_TYPE_CHANGE_FOLDER, ChangeFolderFunc, this);
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
        result =  fPPLib.RemoveEventListener(EVENT_TYPE_CHANGE_FOLDER, ChangeFolderFunc, this);
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
    
    vector<std::pair<string, ai::FilePath>> folders;
    folders.push_back(LoadPresetFromPrefs(MANUFACTURING_PDF_PRESET, PATH_TO_PLANT_MANUFACTURING));
    folders.push_back(LoadPresetFromPrefs(REG_PROOF_PDF_PRESET, PATH_TO_PDFPROOFS));
    folders.push_back(LoadPresetFromPrefs(MICR_PROOF_PDF_PRESET, PATH_TO_PDFPROOFS));
    
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

std::pair<string, ai::FilePath> PrintToPdfFolderPrefsUIController::LoadPresetFromPrefs(string preset, string defaultPath)
{
    PreferenceWriter writer(PRINTTOPDF_FOLDERPREFS_EXTENSION);
    ai::FilePath foundPath;
    
    if ( writer.GetFilePathFromIdentifier(preset, foundPath) )
    {
        return {preset, foundPath};
    }
    else
    {
        return {preset, ai::FilePath(ai::UnicodeString(defaultPath))};
    }
}

void PrintToPdfFolderPrefsUIController::StoreFoldersInPrefs(vector<std::pair<string, ai::FilePath>> folders)
{
    for ( auto folder : folders )
    {
        SaveNewPathForPreset(folder.first, folder.second);
    }
}

bool PrintToPdfFolderPrefsUIController::SaveNewPathForPreset(string preset, ai::FilePath newPath)
{
    PreferenceWriter writer(PRINTTOPDF_FOLDERPREFS_EXTENSION);
    return writer.SetFilePathForIdentifier(preset, newPath);
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

ai::FilePath PrintToPdfFolderPrefsUIController::ChooseNewFolder(string path)
{
    ai::FilePath aiPath((ai::UnicodeString)path);
    
    sAIUser->GetDirectoryDialog(ai::UnicodeString("Choose new output folder"), aiPath);
    
    return aiPath;
}

void PrintToPdfFolderPrefsUIController::SendNewPathToDialog(string preset, ai::FilePath aiPath)
{
    string xmlString;
    xmlString.append("<root>");
    xmlString.append("<folder>");
    xmlString.append("<preset-name>").append(preset).append("</preset-name>");
    xmlString.append("<path>").append(aiPath.GetFullPath().getInStdString(kAIPlatformCharacterEncoding)).append("</path>");
    xmlString.append("</folder>");
    xmlString.append("</root>");
    
    csxs::event::Event event = {
        EVENT_TYPE_FOLDER_BACK,
        csxs::event::kEventScope_Application,
        ILST_APP,
        NULL,
        xmlString.c_str()
    };
    fPPLib.DispatchEvent(&event);
}

//
//  PrintToPdf.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#include "PrintToPdf.h"
#include "PdfSettings.h"
#include "AIDocument.h"
#include <string>

extern AIDocumentSuite* sAIDocument;

bool PrintToPdf()
{
    //Create VPBs for each type of PDF
    VPB vpb(PdfSettings::Manufacturing);
    
    // Save range
    int artboardRange = 1;
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, to_string(artboardRange).c_str());
    
    //Set path
    sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, CreateSaveFilePath(artboardRange).GetFullPath());
    
    // Gather common parameters then save.
    SaveACopyAs(vpb);

    return true;
}

ai::FilePath CreateSaveFilePath(int artboardIndex)
{
    ai::FilePath openedFilePath;
    ai::FilePath saveasFilePath(ai::UnicodeString(DEFAULT_OUTPUTPATH));
    
    sAIDocument->GetDocumentFileSpecification(openedFilePath);
    PlateNumber plateNumber(openedFilePath.GetFileNameNoExt().getInStdString(kAIPlatformCharacterEncoding));
    
    saveasFilePath.AddComponent(ai::UnicodeString(plateNumber.GetPlateNumber()));
    saveasFilePath.AddExtension("pdf");
    
    return saveasFilePath;
}

/*
 */
ASErr SaveACopyAs(VPB& vpb)
{
    ASErr result = kNoErr;
    try {
        // Play the action.
        result = sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, vpb);
        aisdk::check_ai_error(result);
    }
    catch (ai::Error& ex) {
        result = ex;
    }	
    return result;
    
}
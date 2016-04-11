//
//  PrintToPdf.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#include "PrintToPdf.h"
#include "AIDocument.h"

extern AIDocumentSuite* sAIDocument;

bool PrintToPdf()
{
    //Create VPBs for each type of PDF
    VPB vpb(ManufacturingSettings);
    
    // Save range
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, "1");
    
    //Create path
    ai::FilePath openedFilePath;
    ai::FilePath saveasFilePath(ai::UnicodeString(DEFAULT_OUTPUTPATH));
    
    sAIDocument->GetDocumentFileSpecification(openedFilePath);
    PlateNumber plateNumber(openedFilePath.GetFileNameNoExt().getInStdString(kAIPlatformCharacterEncoding));
    
    saveasFilePath.AddComponent(ai::UnicodeString(plateNumber.GetPlateNumber()));
    saveasFilePath.AddExtension("pdf");
    
    sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, saveasFilePath.GetFullPath());

    
    // Gather common parameters then save.
    SaveACopyAs(vpb);

    
    return true;
}

void ManufacturingSettings(AIActionParamValueRef* target)
{
 
    // Format parameter.
    sAIActionManager->AIActionSetString(*target, kAIExportDocumentFormatKey, kAIPDFFileFormat);
    sAIActionManager->AIActionSetString(*target, kAIExportDocumentExtensionKey, kAIPDFFileFormatExtension);
    
    // Option Set
    sAIActionManager->AIActionSetInteger(*target, kAIPDFOptionSetKey, kAIPDFOptionSetCustom);
    sAIActionManager->AIActionSetString(*target, kAIPDFOptionSetNameKey, MANUFACTURING_PDF_PRESET);
    
    // Save multiple artboards
    sAIActionManager->AIActionSetBoolean(*target, kAIExportDocumentSaveMultipleArtboardsKey, TRUE);
    
    // Save all
    sAIActionManager->AIActionSetBoolean(*target, kAIExportDocumentSaveAllKey, FALSE);
    
    /*THESE SHOULD BE SET PROPERLY BY THE joboptions FILE WE'RE USING
     // Compatibility.
     //ASErr result = sAIActionManager->AIActionSetInteger(vpb.fActionParamValueRef, kAIPDFCompatibilityKey, kAIPDFCompatibility14);
     //aisdk::check_ai_error(result);
     
     // Crop To
     //result = sAIActionManager->AIActionSetInteger(vpb.fActionParamValueRef, kAIPDFCropToKey, kAIPDFBleedBox);
     //aisdk::check_ai_error(result);
     
     // Password Protection
     result = sAIActionManager->AIActionSetBoolean(vpb, kAIPDFUserPasswordRequiredKey, FALSE);
     aisdk::check_ai_error(result);
     result = sAIActionManager->AIActionSetBoolean(vpb, kAIPDFMasterPasswordRequiredKey, FALSE);
     aisdk::check_ai_error(result);
     
     // Printing Permissions
     result = sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFPrintingPermKey, "Printing Allowed", kAIPDFPrint128HighResIndex);
     aisdk::check_ai_error(result);
     
     // Changes Permissions
     result = sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFChangesPermKey, "Changes Allowed", kAIPDFChanges128AnyIndex);
     aisdk::check_ai_error(result);
     
     // Enable Copy
     result = sAIActionManager->AIActionSetBoolean(vpb, kAIPDFEnableCopyKey, TRUE);
     aisdk::check_ai_error(result);
     
     // Enable Access
     result = sAIActionManager->AIActionSetBoolean(vpb, kAIPDFEnableAccessKey, TRUE);
     aisdk::check_ai_error(result);
     
     // Enable Copy Access
     result = sAIActionManager->AIActionSetBoolean(vpb, kAIPDFEnableCopyAccessKey, TRUE);
     aisdk::check_ai_error(result);
     
     // Enable Plaintext Metadata
     result = sAIActionManager->AIActionSetBoolean(vpb, kAIPDFEnablePlaintextMetaKey, FALSE);
     aisdk::check_ai_error(result);
     */
    
    // Enable/Disable dialogs
    sAIActionManager->AIActionSetBoolean(*target, kAISaveDocumentAsGetParamsKey, FALSE);
    
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
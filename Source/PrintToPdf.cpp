//
//  PrintToPdf.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#include "PrintToPdf.h"

bool PrintToPdf()
{
    //Create VPBs for each type of PDF
    VPB vpb = VPBFactory(ManufacturingSettings);
    
    // Save range
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, "1");
    
    //Output directory
    NSString* outputFolder;
    outputFolder = @DEFAULT_OUTPUTPATH;
    
    //Create path
    string name = "TESTPLATE#";
    NSString* fullPathForNewPDF = [outputFolder stringByAppendingPathComponent:[[NSString stringWithCString:name.c_str() encoding:NSASCIIStringEncoding] stringByAppendingString: @".pdf"]];
    
    // Name parameter.
    ai::FilePath path;
    path.Set((ai::UnicodeString)[fullPathForNewPDF cStringUsingEncoding:NSASCIIStringEncoding], FALSE);
    ASErr result = sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, path.GetFullPath());
    aisdk::check_ai_error(result);
    
    
    // Gather common parameters then save.
    result = SaveACopyAs(vpb);

    return true;
}

VPB* ManufacturingSettings()
{
    VPB vpb;
    
    // Format parameter.
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentFormatKey, kAIPDFFileFormat);
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentExtensionKey, kAIPDFFileFormatExtension);
    
    // Option Set
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFOptionSetKey, kAIPDFOptionSetCustom);
    sAIActionManager->AIActionSetString(vpb, kAIPDFOptionSetNameKey, MANUFACTURING_PDF_PRESET);
    
    // Save multiple artboards
    sAIActionManager->AIActionSetBoolean(vpb, kAIExportDocumentSaveMultipleArtboardsKey, TRUE);
    
    // Save all
    sAIActionManager->AIActionSetBoolean(vpb, kAIExportDocumentSaveAllKey, FALSE);
    
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
    sAIActionManager->AIActionSetBoolean(vpb, kAISaveDocumentAsGetParamsKey, FALSE);
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
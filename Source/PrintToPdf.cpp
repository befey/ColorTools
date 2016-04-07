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
    //We need to get the plate number from the plate struct
    SaveDocumentAsPDF( "TESTPLATE#" );

    return true;
}

/*
 */
ASErr SaveDocumentAsPDF(const string& name)
{
    ASErr result = kNoErr;
    try {
        SDK_ASSERT(sAIActionManager);
        
        VPB vpb;
        
        // Format parameter.
        result = sAIActionManager->AIActionSetString(vpb, kAIExportDocumentFormatKey, kAIPDFFileFormat);
        aisdk::check_ai_error(result);
        result = sAIActionManager->AIActionSetString(vpb, kAIExportDocumentExtensionKey, kAIPDFFileFormatExtension);
        
        // Option Set
        result = sAIActionManager->AIActionSetInteger(vpb, kAIPDFOptionSetKey, kAIPDFOptionSetCustom);
        aisdk::check_ai_error(result);
        result = sAIActionManager->AIActionSetString(vpb, kAIPDFOptionSetNameKey, "Manufacturing");
        aisdk::check_ai_error(result);
        
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
        
        // Save multiple artboards
        result = sAIActionManager->AIActionSetBoolean(vpb, kAIExportDocumentSaveMultipleArtboardsKey, TRUE);
        aisdk::check_ai_error(result);
        
        // Save all
        result = sAIActionManager->AIActionSetBoolean(vpb, kAIExportDocumentSaveAllKey, FALSE);
        aisdk::check_ai_error(result);
        
        // Save range
        result = sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, "1");
        aisdk::check_ai_error(result);
        
        
        //THESE TWO SHOULD BE SET PROPERLY BY THE joboptions FILE WE'RE USING
        // Compatibility.
        //ASErr result = sAIActionManager->AIActionSetInteger(vpb.fActionParamValueRef, kAIPDFCompatibilityKey, kAIPDFCompatibility14);
        //aisdk::check_ai_error(result);
        
        // Crop To
        //result = sAIActionManager->AIActionSetInteger(vpb.fActionParamValueRef, kAIPDFCropToKey, kAIPDFBleedBox);
        //aisdk::check_ai_error(result);
        
        
        // Enable/Disable dialogs
        result = sAIActionManager->AIActionSetBoolean(vpb, kAISaveDocumentAsGetParamsKey, FALSE);
        
        
        //Output directory
        NSString* outputFolder;
        outputFolder = @DEFAULT_OUTPUTPATH;
        
        //Create path
        NSString* fullPathForNewPDF = [outputFolder stringByAppendingPathComponent:[[NSString stringWithCString:name.c_str() encoding:NSASCIIStringEncoding] stringByAppendingString: @".pdf"]];
        
        // Name parameter.
        ai::FilePath path;
        path.Set((ai::UnicodeString)[fullPathForNewPDF cStringUsingEncoding:NSASCIIStringEncoding], FALSE);
        ASErr result = sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, path.GetFullPath());
        aisdk::check_ai_error(result);
        
        
        // Gather common parameters then save.
        result = SaveACopyAs(vpb);
        aisdk::check_ai_error(result);
    }
    catch (ai::Error& ex) {
        result = ex;
    }
    return result;
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
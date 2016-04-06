/*
 *  VPB.cpp
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 2/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "VPB.h"
#include "SDKErrors.h"


Instance::VPB::VPB() : fActionParamValueRef(NULL)
{
	ASErr result = kNoErr;
	try {
		SDK_ASSERT(sAIActionManager);
		result = sAIActionManager->AINewActionParamValue(&fActionParamValueRef);
		SDK_ASSERT(!result);
		SDK_ASSERT(fActionParamValueRef);
	}
	catch (ai::Error) {
	}
}

/*
*/
Instance::VPB::~VPB()
{
	ASErr result = kNoErr;
	try {
		SDK_ASSERT(sAIActionManager);
		result = sAIActionManager->AIDeleteActionParamValue(fActionParamValueRef);
		SDK_ASSERT(!result);
		fActionParamValueRef = NULL;
	}
	catch (ai::Error) {
	}
}


// kAISaveDocumentAsAction parameters
/*
*/
void Instance::VPB::SetSaveName(const ai::FilePath& name)
{
	SDK_ASSERT(sAIActionManager);
	ASErr result = sAIActionManager->AIActionSetStringUS(this->fActionParamValueRef, kAISaveDocumentAsNameKey, name.GetFullPath());
	aisdk::check_ai_error(result);
}


/*
*/
ASErr Instance::SaveDocumentAsPDF(const string& name)
{
	ASErr result = kNoErr;
	try {
		SDK_ASSERT(sAIActionManager);
		
		Instance::VPB vpb;

		// Format parameter.
        result = sAIActionManager->AIActionSetString(vpb.fActionParamValueRef, kAIExportDocumentFormatKey, kAIPDFFileFormat);
        aisdk::check_ai_error(result);
        result = sAIActionManager->AIActionSetString(vpb.fActionParamValueRef, kAIExportDocumentExtensionKey, kAIPDFFileFormatExtension);
        
        // Option Set
        result = sAIActionManager->AIActionSetInteger(vpb.fActionParamValueRef, kAIPDFOptionSetKey, kAIPDFOptionSetCustom);
        aisdk::check_ai_error(result);
        result = sAIActionManager->AIActionSetString(vpb.fActionParamValueRef, kAIPDFOptionSetNameKey, "Manufacturing");
        aisdk::check_ai_error(result);
        
        // Password Protection
        result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAIPDFUserPasswordRequiredKey, FALSE);
        aisdk::check_ai_error(result);
        result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAIPDFMasterPasswordRequiredKey, FALSE);
        aisdk::check_ai_error(result);
        
        // Printing Permissions
        result = sAIActionManager->AIActionSetEnumerated(vpb.fActionParamValueRef, kAIPDFPrintingPermKey, "Printing Allowed", kAIPDFPrint128HighResIndex);
        aisdk::check_ai_error(result);
        
        // Changes Permissions
        result = sAIActionManager->AIActionSetEnumerated(vpb.fActionParamValueRef, kAIPDFChangesPermKey, "Changes Allowed", kAIPDFChanges128AnyIndex);
        aisdk::check_ai_error(result);
        
        // Enable Copy
        result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAIPDFEnableCopyKey, TRUE);
        aisdk::check_ai_error(result);
        
        // Enable Access
        result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAIPDFEnableAccessKey, TRUE);
        aisdk::check_ai_error(result);
        
        // Enable Copy Access
        result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAIPDFEnableCopyAccessKey, TRUE);
        aisdk::check_ai_error(result);
        
        // Enable Plaintext Metadata
        result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAIPDFEnablePlaintextMetaKey, FALSE);
        aisdk::check_ai_error(result);
        
        // Save multiple artboards
        result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAIExportDocumentSaveMultipleArtboardsKey, TRUE);
        aisdk::check_ai_error(result);
        
        // Save all
        result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAIExportDocumentSaveAllKey, FALSE);
        aisdk::check_ai_error(result);
        
        // Save range
        result = sAIActionManager->AIActionSetString(vpb.fActionParamValueRef, kAIExportDocumentSaveRangeKey, "1");
        aisdk::check_ai_error(result);

        
        //THESE TWO SHOULD BE SET PROPERLY BY THE joboptions FILE WE'RE USING
        // Compatibility.
		//ASErr result = sAIActionManager->AIActionSetInteger(vpb.fActionParamValueRef, kAIPDFCompatibilityKey, kAIPDFCompatibility14);
		//aisdk::check_ai_error(result);
		
		// Crop To
		//result = sAIActionManager->AIActionSetInteger(vpb.fActionParamValueRef, kAIPDFCropToKey, kAIPDFBleedBox);
		//aisdk::check_ai_error(result);
        
        
        // Enable/Disable dialogs
		result = sAIActionManager->AIActionSetBoolean(vpb.fActionParamValueRef, kAISaveDocumentAsGetParamsKey, FALSE);


		//Output directory
		NSString* outputFolder;
        outputFolder = @DEFAULT_OUTPUTPATH;
        
		//Create path
		NSString* fullPathForNewPDF = [outputFolder stringByAppendingPathComponent:[[NSString stringWithCString:name.c_str() encoding:NSASCIIStringEncoding] stringByAppendingString: @".pdf"]];

		// Gather common parameters then save.
		
		result = this->SaveACopyAs((ai::UnicodeString)[fullPathForNewPDF cStringUsingEncoding:NSASCIIStringEncoding], vpb);
		aisdk::check_ai_error(result);
	}
	catch (ai::Error& ex) {
		result = ex;
	}	
	return result;
}

/*
*/
ASErr Instance::SaveACopyAs(const ai::UnicodeString& name, Instance::VPB& vpb)
{
	ASErr result = kNoErr;
	try {
		// Name parameter.
		ai::FilePath path;
		path.Set(name, FALSE);

		vpb.SetSaveName(path);

		// Play the action.
		result = sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, vpb.fActionParamValueRef);
		aisdk::check_ai_error(result);
	}
	catch (ai::Error& ex) {
		result = ex;
	}	
	return result;

}
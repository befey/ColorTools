//
//  PdfSettings.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/12/16.
//
//

#include "PdfSettings.h"
#include "document.h"


PdfSettings::PdfSettings(SettingsFunction f, string r, bool s) : settingsFunc(f), range(r), separateFiles(s)
{
  ////****** Setup common parameters for all PDFs
    // Format parameter.
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentFormatKey, kAIPDFFileFormat);
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentExtensionKey, kAIPDFFileFormatExtension);
    
    // Enable/Disable dialogs
    sAIActionManager->AIActionSetBoolean(vpb, kAISaveDocumentAsGetParamsKey, FALSE);
  ////*******
    
    // Apply settings passed in
    settingsFunc(vpb);
    
    // Fill Plate Number object
    ai::FilePath openedFilePath;
    sAIDocument->GetDocumentFileSpecification(openedFilePath);
    plateNumber = new PlateNumber(openedFilePath.GetFileNameNoExt().getInStdString(kAIPlatformCharacterEncoding));
    
    // Generate output path
    outputPath = CreateSaveFilePath();
}

PdfSettings PdfSettings::MakePdfSettingsFromXml(const char* xmlData)
{
    using namespace rapidjson;
    
    Document d;
    d.Parse(xmlData);
    
    Value& v = d[PRESET_SELECT];
    SettingsFunction func = GetSettingsFuncForPdfPreset(static_cast<PdfPreset>(v.GetInt()));
    
    v = d[ALLPAGES_CHECK];
    bool allPages = (v.GetBool());
    
    string r;
    if (allPages)
    {
        r = "";
    }
    else
    {
        v = d[RANGE_TEXT];
        r = v.GetString();
    }
    
    return PdfSettings(func, r);
}

void PdfSettings::Print() const
{
    ASErr result;
    ai::FilePath pathToPdfFile = outputPath;
    
    if (!separateFiles) {
        pathToPdfFile.AddComponent(ai::UnicodeString(plateNumber->GetPlateNumber()));
        pathToPdfFile.AddExtension("pdf");
        
        // Set Path
        sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, outputPath.GetFullPath());
        
        // Set Range
        sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, string(range).c_str());
        
        try {
            // Play the action.
            result = sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, vpb);
            aisdk::check_ai_error(result);
        }
        catch (ai::Error& ex) {
            result = ex;
        }
    }
    else
    {
        AIArtboardRangeIterator iter;
        sAIArtboardRange->Begin(range, &iter);
        ai::int32 index = 0;
        while ( kEndOfRangeErr != sAIArtboardRange->Next(iter, &index) ) {
            pathToPdfFile.AddComponent(ai::UnicodeString(plateNumber->GetPlateNumber()));
            
            string token = CreateToken(index);
            if (token != "")
            {
                pathToPdfFile.AddExtension(ai::UnicodeString(token));
            }
            
            pathToPdfFile.AddExtension("pdf");
            string fp = outputPath.GetFullPath().as_UTF8();
            // Set Path
            sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, outputPath.GetFullPath());
            
            // Set Range
            sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, to_string(index+1).c_str());
            
            try {
                // Play the action.
                result = sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, vpb);
                aisdk::check_ai_error(result);
            }
            catch (ai::Error& ex) {
                result = ex;
            }

            pathToPdfFile.RemoveComponent();
        }
        
        sAIArtboardRange->DisposeIterator(iter);
    }
    
}

ai::FilePath PdfSettings::CreateSaveFilePath()
{
    ai::UnicodeString fpUS = ai::UnicodeString(DEFAULT_OUTPUTPATH);
    ai::FilePath saveasFilePath(fpUS);
    
    return saveasFilePath;
}

string PdfSettings::CreateToken(int artboardIndex) const
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties abProps;
    sAIArtboard->GetArtboardProperties(abList, artboardIndex, abProps);
    ai::UnicodeString abName;
    abProps.GetName(abName);
    string abNameS = abName.getInStdString(kAIPlatformCharacterEncoding);
    
    AIBoolean isDefaultName;
    sAIArtboard->IsDefaultName(abProps, isDefaultName);
    
    if (isDefaultName || abNameS == NO_TOKEN_DESIG) {
        return "";
    }
    else
    {
        return abNameS;
    }
}

SettingsFunction PdfSettings::GetSettingsFuncForPdfPreset(PdfPreset preset)
{
    switch (preset) {
        case PdfPreset::Manufacturing:
            return ManufacturingSettingsFunc;
        case PdfPreset::Proof:
            return ProofSettingsFunc;
        case PdfPreset::MicrProof:
            return ProofSettingsFunc;
        default:
            return ManufacturingSettingsFunc;
    }
}

/**************************************************************************
 **************************************************************************/

void ManufacturingSettingsFunc(AIActionParamValueRef target)
{
    // Option Set
    sAIActionManager->AIActionSetInteger(target, kAIPDFOptionSetKey, kAIPDFOptionSetCustom);
    sAIActionManager->AIActionSetString(target, kAIPDFOptionSetNameKey, PdfSettings::MANUFACTURING_PDF_PRESET);
    
    // Save multiple artboards
    sAIActionManager->AIActionSetBoolean(target, kAIExportDocumentSaveMultipleArtboardsKey, TRUE);
    
    // Save all
    sAIActionManager->AIActionSetBoolean(target, kAIExportDocumentSaveAllKey, FALSE);
}

void ProofSettingsFunc(AIActionParamValueRef target)
{
    
}

void MicrProofSettingsFunc(AIActionParamValueRef target)
{
    
}


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

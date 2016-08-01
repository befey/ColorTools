//
//  PdfSettings.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/12/16.
//
//

#include "PdfSettings.h"
#include "PdfResults.h"
#include "document.h"
#include "Plate.h"
#include "SafeguardJobFile.h"

using PrintToPdf::PdfSettings;
using PrintToPdf::PdfPreset;
using PrintToPdf::PdfResults;
using SafeguardFile::SafeguardJobFile;
using SafeguardFile::ProductType;

PdfSettings::PdfSettings(PdfPreset p, string r, bool s) : preset(p), range(r), separateFiles(s)
{
    if (preset == PrintToPdf::PdfPreset::Manufacturing)
    {
        pwRetriever = unique_ptr<PasswordRetriever> { make_unique<NonePasswordRetriever>() };
    }
    else if (preset == PrintToPdf::PdfPreset::Proof)
    {
        pwRetriever = unique_ptr<PasswordRetriever> { make_unique<ProofPasswordRetriever>() };
    }
    else if (preset == PrintToPdf::PdfPreset::MicrProof)
    {
        pwRetriever = unique_ptr<PasswordRetriever> { make_unique<MicrPasswordRetriever>() };
    }
    
    SetPasswords();
    
    SafeguardJobFile sgJobFile;
    ProductType pType = sgJobFile.GetPlateNumber().GetProductType();
    if ( (pType == ProductType::CutSheet || pType == ProductType::Snapset ) && (preset == PdfPreset::MicrProof || preset == PdfPreset::Proof) )
    {
        SetBleeds(AIRealRect{0,0,0,0});
    }
    else
    {
        SetBleeds(sgJobFile.GetBleeds());
    }
        
    SetVpbRange(range);
    
  ////****** Setup common parameters for all PDFs
    // Format parameter.
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentFormatKey, kAIPDFFileFormat);
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentExtensionKey, kAIPDFFileFormatExtension);
    
    // Save multiple artboards
    sAIActionManager->AIActionSetBoolean(vpb, kAIExportDocumentSaveMultipleArtboardsKey, TRUE);
    
    // Save all
    sAIActionManager->AIActionSetBoolean(vpb, kAIExportDocumentSaveAllKey, FALSE);
    
    // Enable/Disable dialogs
    sAIActionManager->AIActionSetBoolean(vpb, kAISaveDocumentAsGetParamsKey, FALSE);
    
    //Turn off preserve Illustrator editability
    sAIActionManager->AIActionSetBoolean(vpb, kAIPDFRoundTripKey, FALSE);
    
    //Turn off Acrobat layers
    sAIActionManager->AIActionSetBoolean(vpb, kAIPDFGenerateAcrobatLayersKey, FALSE);
  ////*******
}

PdfSettings PdfSettings::MakePdfSettingsFromXml(const char* xmlData)
{
    using namespace rapidjson;
    
    Document d;
    d.Parse(xmlData);
    
    Value& v = d[PrintToPdfUIController::PRESET_SELECT];
    PrintToPdf::PdfPreset preset = static_cast<PrintToPdf::PdfPreset>(v.GetInt());
    
    v = d[PrintToPdfUIController::ALLPAGES_CHECK];
    bool allPages = (v.GetBool());
    
    string r;
    if (allPages)
    {
        r = "";
    }
    else
    {
        v = d[PrintToPdfUIController::RANGE_TEXT];
        r = v.GetString();
    }
    
    v = d[PrintToPdfUIController::SEPARATEFILES_CHECK];
    bool separateFiles = (v.GetBool());
    
    return PdfSettings(preset, r, separateFiles);
}

void PdfSettings::SetPasswords()
{
    if (pwRetriever->IsValid())
    {
        // Apply a password if one is required
        if (pwRetriever->GetUserPassword() != "")
        {
            sAIActionManager->AIActionSetBoolean(vpb, kAIPDFUserPasswordRequiredKey, TRUE);
            sAIActionManager->AIActionSetString(vpb, kAIPDFUserPasswordKey, pwRetriever->GetUserPassword().c_str());
        }
        else
        {
            sAIActionManager->AIActionSetBoolean(vpb, kAIPDFUserPasswordRequiredKey, FALSE);
        }
        
        if (pwRetriever->GetMasterPassword() != "")
        {
            sAIActionManager->AIActionSetBoolean(vpb, kAIPDFMasterPasswordRequiredKey, TRUE);
            sAIActionManager->AIActionSetString(vpb, kAIPDFMasterPasswordKey, pwRetriever->GetMasterPassword().c_str());
        }
        else
        {
            sAIActionManager->AIActionSetBoolean(vpb, kAIPDFMasterPasswordRequiredKey, FALSE);
        }
    }
    else
    {
        sAIActionManager->AIActionSetBoolean(vpb, kAIPDFUserPasswordRequiredKey, FALSE);
        sAIActionManager->AIActionSetBoolean(vpb, kAIPDFMasterPasswordRequiredKey, FALSE);
    }
}

void PdfSettings::SetBleeds(AIRealRect bleeds) const
{
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFBleedTopKey, bleeds.top);
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFBleedBottomKey, bleeds.bottom);
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFBleedLeftKey, bleeds.left);
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFBleedRightKey, bleeds.right);
    sAIActionManager->AIActionSetBoolean(vpb, kAIPDFDocBleedKey, FALSE);
}

void PdfSettings::SetPath(ai::FilePath path) const
{
    sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, path.GetFullPath());
}

void PdfSettings::SetVpbRange(string vpbRange) const
{
    sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, vpbRange.c_str());
}
/**************************************************************************
 **************************************************************************/


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
 result = sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFChangesPermKey, "Changes Allowed", kAIPDFChanges128NoneIndex);
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

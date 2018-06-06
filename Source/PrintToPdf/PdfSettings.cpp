//
//  PdfSettings.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/12/16.
//
//

#include "PdfSettings.h"
#include "Plate.h"
#include "SafeguardJobFile.h"
#include "SafeguardJobFileDTO.hpp"
#include "GetIllustratorErrorCode.h"

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/external/rapidjson/document.h"

using PrintToPdf::PdfSettings;
using PrintToPdf::PdfPreset;
using SafeguardFile::SafeguardJobFile;
using SafeguardFile::ProductType;

PdfSettings::PdfSettings(PrintToPdf::PdfPreset p, AIReal bleedAmount, string r, bool s, bool dnd, bool uof)
: PdfSettings(p, r, s, dnd, uof)
{
    SetBleeds(AIRealRect{bleedAmount,bleedAmount,bleedAmount,bleedAmount});
}

PdfSettings::PdfSettings(PdfPreset p, string r, bool s, bool dnd, bool uof)
:
preset(p),
range(r),
separateFiles(s),
doNotDelete(dnd),
userOutputFolder(uof)
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
    //Set High Quality Print settings
    sAIActionManager->AIActionSetString(vpb, kAIPDFOptionSetNameKey, "[High Quality Print]");
    
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
    
    //Turn off Compress art
    sAIActionManager->AIActionSetBoolean(vpb, kAIPDFCompressArtKey, FALSE);
    
    //Turn off all downsampling and set image compression
    sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFColorImageCompressionKindKey, "Color Compression Kind", kAIPDFImageCompressionZIP8bit);
    sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFColorImageResampleKindKey, "Color Resample Kind", kAIPDFNoDownsampling);
    sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFGrayscaleImageCompressionKindKey, "Gray Compression Kind", kAIPDFImageCompressionZIP8bit);
    sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFGrayscaleImageResampleKindKey, "Gray Resample Kind", kAIPDFNoDownsampling);
    sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFMonochromeImageCompressionKindKey, "Mono Compression Kind", kAIPDFMonochromeImageCompressionZIP);
    sAIActionManager->AIActionSetEnumerated(vpb, kAIPDFMonochromeImageResampleKindKey, "Mono Resample Kind", kAIPDFNoDownsampling);
  ////*******
}

PdfSettings PdfSettings::MakePdfSettingsFromJson(const char* json)
{
    using namespace rapidjson;
    
    Document d;
    d.Parse(json);
    
    Value& v = d[PrintToPdfUIController::PRESET_SELECT];
    PrintToPdf::PdfPreset preset = static_cast<PrintToPdf::PdfPreset>(v.GetInt());
        
    v = d[PrintToPdfUIController::DTO];
    string dtoString = (v.GetString());
    PlateBleedInfo::SafeguardJobFileDTO printToPdfDTO;
    std::istringstream is(dtoString);
    {
        try
        {
            cereal::JSONInputArchive iarchive(is); // Create an input archive
            iarchive(printToPdfDTO);
        }
        catch (std::runtime_error e)
        {
            string s(e.what());
        }
    }
    
    string r;
    for (auto artboard : printToPdfDTO.GetPlateDTOs() )
    {
        if (artboard.shouldPrint)
        {
            r += std::to_string(artboard.artboardIndex+1);
            r += ",";
        }
    }
    if (r.length() > 0)
    {
        r.pop_back(); //Remove trailing comma
    }
    
    v = d[PrintToPdfUIController::SEPARATEFILES_CHECK];
    bool separateFiles = (v.GetBool());
    
    v = d[PrintToPdfUIController::DONOTDELETE_CHECK];
    bool doNotDelete = (v.GetBool());
    
    v = d[PrintToPdfUIController::USEROUTPUTFOLDER_CHECK];
    bool userOutputFolder = (v.GetBool());
    
    v = d[PrintToPdfUIController::CUSTOMBLEEDS_TEXT];
    AIReal customBleedAmount = GetBleedAmountFromString(string(v.GetString()));
    if (customBleedAmount != -1)
    {
        return PdfSettings(preset, customBleedAmount, r, separateFiles, doNotDelete, userOutputFolder);
    }

    return PdfSettings(preset, r, separateFiles, doNotDelete, userOutputFolder);
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

AIReal PdfSettings::GetBleedAmountFromString(string expr)
{
    AIExpressionOptions options =
    {
        .unit = kAIPointUnits,
        .minValue = -1,
        .maxValue = kAIRealMax,
        .oldValue = -1,
        .precision = 0
    };
    
    ai::UnicodeString evaluatedExpr;
    AIBoolean isChanged;
    AIDouble numericValue = 999;
    sAIUser->EvaluateExpression(ai::UnicodeString(expr), options, evaluatedExpr, isChanged, numericValue);
    
    return numericValue; //-1 if error
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

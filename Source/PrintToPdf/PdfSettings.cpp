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

using PrintToPdf::PdfSettings;
using PrintToPdf::PdfPreset;
using PrintToPdf::PdfResults;

PdfSettings::PdfSettings(ai::FilePath path, PdfPreset p, unique_ptr<PasswordRetriever> pwRet, unique_ptr<SavePathCreator> spCre, string r, bool s)
: preset(p), pwRetriever(move(pwRet)), spCreator(move(spCre)), range(r), separateFiles(s), plateNumber(new PlateNumber(path.GetFileNameNoExt().getInStdString(kAIPlatformCharacterEncoding)))
{
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
  ////*******
    
    AIRealRect bleed = CalculateBleeds();
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFBleedTopKey, bleed.top);
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFBleedBottomKey, bleed.bottom);
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFBleedLeftKey, bleed.left);
    sAIActionManager->AIActionSetInteger(vpb, kAIPDFBleedRightKey, bleed.right);
    sAIActionManager->AIActionSetBoolean(vpb, kAIPDFDocBleedKey, FALSE);
    
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
    
    // Generate output path
    outputPath = spCreator->GetAiFilePath(*plateNumber);
}

PdfSettings PdfSettings::MakePdfSettingsFromXml(const char* xmlData)
{
    using namespace rapidjson;
    
    Document d;
    d.Parse(xmlData);
    
    unique_ptr<PasswordRetriever> pwr;
    
    Value& v = d[PrintToPdfUIController::PRESET_SELECT];
    PrintToPdf::PdfPreset preset = static_cast<PrintToPdf::PdfPreset>(v.GetInt());
    if ( preset == PrintToPdf::PdfPreset::Manufacturing)
    {
        pwr.reset(new NonePasswordRetriever());
    }
    else if (preset == PrintToPdf::PdfPreset::Proof)
    {
        pwr.reset(new ProofPasswordRetriever());
    }
    else if (preset == PrintToPdf::PdfPreset::MicrProof)
    {
        pwr.reset(new MicrPasswordRetriever());
    }
    
    unique_ptr<SavePathCreator> spc = make_unique<TestingSavePathCreator>();
    
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
    
    ai::FilePath openedFilePath;
    sAIDocument->GetDocumentFileSpecification(openedFilePath);
    
    return PdfSettings(openedFilePath, preset, move(pwr), move(spc), r, separateFiles);
}

PdfResults PdfSettings::Print() const
{
    ASErr result;
    ai::FilePath pathToPdfFile = outputPath;
    PdfResults transactions;

    if (!separateFiles)
    {
        pathToPdfFile.AddComponent(ai::UnicodeString(plateNumber->GetPlateNumber()));
        pathToPdfFile.AddExtension("pdf");
        
        // Set Path
        sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, pathToPdfFile.GetFullPath());
        
        // Set Range
        sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, string(range).c_str());
        
        try
        {
            // Play the action.
            result = sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, vpb);
            aisdk::check_ai_error(result);
        }
        catch (ai::Error& ex)
        {
            result = ex;
        }
        
        transactions.AddResult({PdfResults::Transaction::Created, pathToPdfFile});
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
            string fp = pathToPdfFile.GetFullPath().as_UTF8();
            // Set Path
            sAIActionManager->AIActionSetStringUS(vpb, kAISaveDocumentAsNameKey, pathToPdfFile.GetFullPath());
            
            // Set Range
            sAIActionManager->AIActionSetString(vpb, kAIExportDocumentSaveRangeKey, to_string(index+1).c_str());
            
            try
            {
                // Play the action.
                result = sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, vpb);
                aisdk::check_ai_error(result);
            }
            catch (ai::Error& ex)
            {
                result = ex;
            }
            
            transactions.AddResult({PdfResults::Transaction::Created, pathToPdfFile});

            pathToPdfFile.RemoveComponent();
        }
        
        sAIArtboardRange->DisposeIterator(iter);
    }
    return transactions;
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
    
    if (isDefaultName || abNameS == NO_TOKEN_DESIG)
    {
        return "";
    }
    else
    {
        return abNameS;
    }
}

AIRealRect PdfSettings::CalculateBleeds()
{
    AIRealRect bleedRect;
    
    PlateNumber::ProductType pt = plateNumber->GetProductType();
    
    if (pt == PlateNumber::ProductType::CutSheet)
    {
        if (preset == PdfPreset::Manufacturing)
        {
            sAIRealMath->AIRealRectSet(&bleedRect, 36, 36, 36, 36);
        }
        else
        {
            sAIRealMath->AIRealRectSet(&bleedRect, 0, 0, 0, 0);
        }
    }
    else if (pt == PlateNumber::ProductType::BusinessStat)
    {
        if (preset == PdfPreset::Manufacturing)
        {
            sAIRealMath->AIRealRectSet(&bleedRect, 12, 12, 12, 12);
        }
    }
    else //Continuous and Snapsets
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 0, 0, 0, 0);
    }
    
    return bleedRect;
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

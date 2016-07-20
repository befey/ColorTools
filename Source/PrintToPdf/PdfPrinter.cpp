//
//  PdfPrinter.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#include "SafeguardToolsPlugin.h"
#include "PdfPrinter.h"
#include "PdfSettings.h"
#include "PdfResults.h"

using PrintToPdf::PdfPrinter;
using PrintToPdf::SingleFilePdfPrinter;
using PrintToPdf::SeparateFilePdfPrinter;
using PrintToPdf::PdfSettings;
using PrintToPdf::PdfResults;
using SafeguardFile::PlateNumber;
using SafeguardFile::ProductType;

SingleFilePdfPrinter::SingleFilePdfPrinter(const PdfPreset preset) : PdfPrinter(preset) {}
SeparateFilePdfPrinter::SeparateFilePdfPrinter(const PdfPreset preset) : PdfPrinter(preset) {}

PdfPrinter::PdfPrinter(const PdfPreset preset)
{
    pathCreator = make_unique<PathCreator>();
    
    efDeleter = make_unique<ExistingFileDeleter>();
    tpConverter = make_unique<TypeToPathsConverter>();
    
    //SETUP LAYER VISIBILITY
    if (GetPlateNumber().GetProductType() == ProductType::BusinessStat)
    {
        layerVisibility = unique_ptr<LayerVisibility> { make_unique<BStatLayerVisibility>() };
    }
    else
    {
        layerVisibility = unique_ptr<LayerVisibility> { make_unique<LaserLayerVisibility>() };
    }
    
    //SETUP PATH BUILDER
    if (preset == PdfPreset::Manufacturing)
    {
        pathBuilder = unique_ptr<PathBuilder> { make_unique<ManufacturingPathBuilder>() };
    }
    else if (preset == PdfPreset::MicrProof)
    {
        pathBuilder = unique_ptr<PathBuilder> { make_unique<MicrProofPathBuilder>() };
    }
    else if (preset == PdfPreset::Proof)
    {
        pathBuilder = unique_ptr<PathBuilder> { make_unique<ProofPathBuilder>() };
    }
    
    outputPath = pathBuilder->GetAiFilePath(GetPlateNumber());
}

unique_ptr<PdfPrinter> PdfPrinter::GetPrinter(const PdfPreset preset, const bool separateFiles)
{
    if (separateFiles)
    {
        return unique_ptr<PdfPrinter> { make_unique<SeparateFilePdfPrinter>(preset) };
    }
    else
    {
        return unique_ptr<PdfPrinter> { make_unique<SingleFilePdfPrinter>(preset) };
    }
}

const PlateNumber PdfPrinter::GetPlateNumber() const
{
    SafeguardJobFile sgJobFile;
    return sgJobFile.GetPlateNumber();
}

const string PdfPrinter::GetToken(int plateIndex) const
{
    SafeguardJobFile sgJobFile;
    return sgJobFile.GetToken(plateIndex);
}

PdfResults PdfPrinter::Print(const PdfSettings& settings) const
{
    PdfResults transactions;
    
    if ( pathCreator->CreatePath(outputPath) )
    {
        transactions.AddResult(efDeleter->Delete(GetPlateNumber(), outputPath));
        
        layerVisibility->SetLayerVisibility();
        tpConverter->ConvertTypeToPaths();
        
        transactions.AddResult(CustomPrintSteps(settings));
    }   
    
    return transactions;
}

PdfResults SingleFilePdfPrinter::CustomPrintSteps(const PdfSettings& settings) const
{
    PdfResults transactions;
    
    ai::FilePath pathToPdfFile = outputPath;
    
    if (GetPlateNumber().IsValid())
    {
        pathToPdfFile.AddComponent(ai::UnicodeString(GetPlateNumber()));
    }
    else
    {
        ai::FilePath openedFilePath;
        sAIDocument->GetDocumentFileSpecification(openedFilePath);
        pathToPdfFile.AddComponent(openedFilePath.GetFileNameNoExt());
    }
    
    pathToPdfFile.AddExtension("pdf");
    
    // Set Path
    settings.SetPath(pathToPdfFile);
        
    sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, settings);
    
    transactions.AddResult({PdfResults::Transaction::Created, pathToPdfFile});
    
    return transactions;
}

PdfResults SeparateFilePdfPrinter::CustomPrintSteps(const PdfSettings& settings) const
{
    PdfResults transactions;
    
    AIArtboardRangeIterator iter;
    sAIArtboardRange->Begin(settings.GetRange(), &iter);
    ai::int32 index = 0;
    
    ai::FilePath pathToPdfFile = outputPath;
    
    while ( kEndOfRangeErr != sAIArtboardRange->Next(iter, &index) )
    {
        if (GetPlateNumber().IsValid())
        {
            pathToPdfFile.AddComponent(ai::UnicodeString(GetPlateNumber()));
        }
        else
        {
            ai::FilePath openedFilePath;
            sAIDocument->GetDocumentFileSpecification(openedFilePath);
            pathToPdfFile.AddComponent(openedFilePath.GetFileNameNoExt());
        }
        
        string token = GetToken(index);
        if (token != "")
        {
            pathToPdfFile.AddExtension(ai::UnicodeString(token));
        }
        
        pathToPdfFile.AddExtension("pdf");
        // Set Path
        settings.SetPath(pathToPdfFile);
        
        // Set Range
        settings.SetVpbRange(to_string(index+1));
        
        sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, settings);
        
        transactions.AddResult({PdfResults::Transaction::Created, pathToPdfFile});
        
        pathToPdfFile.RemoveComponent();
    }
    
    sAIArtboardRange->DisposeIterator(iter);
    
    return transactions;
}
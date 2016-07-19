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

SingleFilePdfPrinter::SingleFilePdfPrinter() : PdfPrinter() {}
SeparateFilePdfPrinter::SeparateFilePdfPrinter() : PdfPrinter() {}

PdfPrinter::PdfPrinter()
{
    pathBuilder = unique_ptr<PathBuilder> { make_unique<TestingPathBuilder>() };
    outputPath = pathBuilder->GetAiFilePath(GetPlateNumber());
    
    pathCreator = unique_ptr<PathCreator>();
    
    efDeleter = unique_ptr<ExistingFileDeleter>();
    tpConverter = unique_ptr<TypeToPathsConverter>();
    
    if (GetPlateNumber().GetProductType() == ProductType::BusinessStat)
    {
        layerVisibility = unique_ptr<LayerVisibility> { make_unique<BStatLayerVisibility>() };
    }
    else
    {
        layerVisibility = unique_ptr<LayerVisibility> { make_unique<LaserLayerVisibility>() };
    }
}

unique_ptr<PdfPrinter> PdfPrinter::GetPrinter(const bool separateFiles)
{
    if (separateFiles)
    {
        return unique_ptr<PdfPrinter> { make_unique<SeparateFilePdfPrinter>() };
    }
    else
    {
        return unique_ptr<PdfPrinter> { make_unique<SingleFilePdfPrinter>() };
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
        tpConverter->ConvertTypeToPaths(settings.GetRange());
        
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
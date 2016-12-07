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
#include "CurrentFilenameRetriever.hpp"
#include "TokenCreator.hpp"

using PrintToPdf::PdfPrinter;
using PrintToPdf::SingleFilePdfPrinter;
using PrintToPdf::SeparateFilePdfPrinter;
using PrintToPdf::PdfSettings;
using PrintToPdf::PdfResults;
using SafeguardFile::ProductType;

SingleFilePdfPrinter::SingleFilePdfPrinter(const PdfPreset preset) : PdfPrinter(preset) {}
SeparateFilePdfPrinter::SeparateFilePdfPrinter(const PdfPreset preset) : PdfPrinter(preset) {}

PdfPrinter::PdfPrinter(const PdfPreset preset)
{
    pathCreator = make_unique<PathCreator>();
    
    efDeleter = make_unique<ExistingFileDeleter>();
    tpConverter = make_unique<TypeToPathsConverter>();
    
    plateNumber = SafeguardFile::PlateNumber(CurrentFilenameRetriever::GetFilenameNoExt());
    
    layerVisibility = LayerVisibility::GetLayerVisibility(plateNumber.GetProductType(), preset);
        
    pathBuilder = PathBuilder::GetPathBuilder(preset);
    outputPath = pathBuilder->GetAiFilePath(plateNumber);
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

PdfResults PdfPrinter::Print(const PdfSettings& settings)
{
    unique_ptr<PdfPrinter> printer = GetPrinter(settings.Preset(), settings.SeparateFiles());
    
    return printer->DoIt(settings);
}

PdfResults PdfPrinter::DoIt(const PdfSettings& settings) const
{
    PdfResults transactions;
    
    if ( pathCreator->CreatePath(outputPath) )
    {
        transactions.AddResult(efDeleter->Delete(plateNumber, outputPath));
        
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
    
    pathToPdfFile.AddComponent(ai::UnicodeString(plateNumber));
    
    pathToPdfFile.AddExtension("pdf");
    
    // Set Path
    settings.Path(pathToPdfFile);
        
    sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, settings);
    
    transactions.AddResult({PdfResults::Transaction::Created, pathToPdfFile});
    
    return transactions;
}

PdfResults SeparateFilePdfPrinter::CustomPrintSteps(const PdfSettings& settings) const
{
    PdfResults transactions;
    
    AIArtboardRangeIterator iter;
    sAIArtboardRange->Begin(settings.Range(), &iter);
    ai::int32 index = 0;
    
    ai::FilePath pathToPdfFile = outputPath;
    
    while ( kEndOfRangeErr != sAIArtboardRange->Next(iter, &index) )
    {
        PdfSettings singlePageSettings(settings);
        
        pathToPdfFile.AddComponent(ai::UnicodeString(plateNumber));
        
        string token = SafeguardFile::TokenCreator(plateNumber, index).GetToken();
        if (token != "")
        {
            pathToPdfFile.AddExtension(ai::UnicodeString(token));
        }
        
        pathToPdfFile.AddExtension("pdf");
        // Set Path
        string path = pathToPdfFile.GetFullPath().as_Platform();
        singlePageSettings.Path(pathToPdfFile);
        
        // Set Range
        singlePageSettings.Range(to_string(index+1));
        
        sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, singlePageSettings);
        
        transactions.AddResult({PdfResults::Transaction::Created, pathToPdfFile});
        
        pathToPdfFile.RemoveComponent();
    }
    
    sAIArtboardRange->DisposeIterator(iter);
    
    return transactions;
}

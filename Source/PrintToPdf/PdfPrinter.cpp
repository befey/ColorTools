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

SingleFilePdfPrinter::SingleFilePdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder) : PdfPrinter(preset, doNotDelete, userOutputFolder) {}
SeparateFilePdfPrinter::SeparateFilePdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder) : PdfPrinter(preset, doNotDelete, userOutputFolder) {}

PdfPrinter::PdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder)
{
    pathCreator = make_unique<PathCreator>();
    
    efDeleter = ExistingFileDeleter::GetExistingFileDeleter(doNotDelete);
    
    tpConverter = make_unique<TypeToPathsConverter>();
    
    plateNumber = SafeguardFile::PlateNumber(CurrentFilenameRetriever::GetFilenameNoExt());
    
    layerVisibility = LayerVisibility::GetLayerVisibility(plateNumber.GetProductType(), preset);
        
    pathBuilder = PathBuilder::GetPathBuilder(preset, userOutputFolder);
    outputPath = pathBuilder->GetAiFilePath(plateNumber);
}

unique_ptr<PdfPrinter> PdfPrinter::GetPrinter(const PdfPreset preset, const bool separateFiles, const bool doNotDelete, const bool userOutputFolder)
{
    if (separateFiles)
    {
        return unique_ptr<PdfPrinter> { make_unique<SeparateFilePdfPrinter>(preset, doNotDelete, userOutputFolder) };
    }
    else
    {
        return unique_ptr<PdfPrinter> { make_unique<SingleFilePdfPrinter>(preset, doNotDelete, userOutputFolder) };
    }
}

PdfResults PdfPrinter::Print(const PdfSettings& settings)
{
    unique_ptr<PdfPrinter> printer = GetPrinter(settings.GetPreset(), settings.OutputSeparateFiles(), settings.DoNotDelete(), settings.UserOutputFolder());
    
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
        pathToPdfFile.AddComponent(ai::UnicodeString(plateNumber));
        
        string token = SafeguardFile::TokenCreator(plateNumber, index).GetToken();
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

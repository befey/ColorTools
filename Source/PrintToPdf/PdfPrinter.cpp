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
#include "CurrentFilenameRetriever.hpp"
#include "TokenCreator.hpp"

using PrintToPdf::PdfPrinter;
using PrintToPdf::SingleFilePdfPrinter;
using PrintToPdf::SeparateFilePdfPrinter;
using PrintToPdf::PdfSettings;
using SafeguardFile::ProductType;

SingleFilePdfPrinter::SingleFilePdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder) : PdfPrinter(preset, doNotDelete, userOutputFolder) {}
SeparateFilePdfPrinter::SeparateFilePdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder) : PdfPrinter(preset, doNotDelete, userOutputFolder) {}

PdfPrinter::PdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder)
{
    plateNumber = SafeguardFile::PlateNumber(CurrentFilenameRetriever::GetFilenameNoExt());
    
    pathBuilder = PathBuilder::GetPathBuilder(preset, userOutputFolder);
    outputPath = pathBuilder->GetAiFilePath(plateNumber);
    
    printCommand.AddCommand(make_shared<CreatePathCommand>(outputPath, true, transactions));
    
    if (!doNotDelete)
    {
        printCommand.AddCommand(make_shared<DeleteFilesMatchingCommand>(outputPath, plateNumber, false, transactions));
    }
    
    printCommand.AddCommand(make_shared<SetLayerVisibilityCommand>(plateNumber.GetProductType(), preset, false));
    
    printCommand.AddCommand(make_shared<ConvertTypeToPathsCommand>(false));
    
    if (plateNumber.GetProductType() == SafeguardFile::ProductType::BusinessStat)
    {
        printCommand.AddCommand(make_shared<MakeKeylineBoundingBox>(false));
    }
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

FilesystemResults PdfPrinter::Print(const PdfSettings& settings)
{
    unique_ptr<PdfPrinter> printer = GetPrinter(settings.GetPreset(), settings.OutputSeparateFiles(), settings.DoNotDelete(), settings.UserOutputFolder());
    
    return printer->DoIt(settings);
}

FilesystemResults PdfPrinter::DoIt(const PdfSettings& settings)
{    
    if ( printCommand.Execute() )
    {        
        transactions.AddResult(CustomPrintSteps(settings));
    }
    
    return transactions;
}

FilesystemResults SingleFilePdfPrinter::CustomPrintSteps(const PdfSettings& settings) const
{
    FilesystemResults transactions;
    
    ai::FilePath pathToPdfFile = outputPath;
    
    pathToPdfFile.AddComponent(ai::UnicodeString(plateNumber));
    
    pathToPdfFile.AddExtension("pdf");
    
    // Set Path
    settings.SetPath(pathToPdfFile);
        
    AIErr error = sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, settings);
    
    if (error == kNoErr)
    {
        transactions.AddResult({FilesystemResults::Transaction::Created, pathToPdfFile});
    }
    
    return transactions;
}

FilesystemResults SeparateFilePdfPrinter::CustomPrintSteps(const PdfSettings& settings) const
{
    FilesystemResults transactions;
    
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
        
        AIErr error = sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, settings);
        
        if (error == kNoErr)
        {
            transactions.AddResult({FilesystemResults::Transaction::Created, pathToPdfFile});
        }
        
        pathToPdfFile.RemoveComponent();
    }
    
    sAIArtboardRange->DisposeIterator(iter);
    
    return transactions;
}

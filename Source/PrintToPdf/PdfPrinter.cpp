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

SingleFilePdfPrinter::SingleFilePdfPrinter(shared_ptr<PdfSettings> settings) : PdfPrinter( move(settings) ) {}
SeparateFilePdfPrinter::SeparateFilePdfPrinter(shared_ptr<PdfSettings> settings) : PdfPrinter( move(settings) ) {}

PdfPrinter::PdfPrinter(shared_ptr<PdfSettings> settings)
{
    pdfSettings = settings;
    
    pathBuilder = unique_ptr<PathBuilder> { make_unique<TestingPathBuilder>() };
    outputPath = pathBuilder->GetAiFilePath(GetPlateNumber());
    
    pathCreator = unique_ptr<PathCreator>();
    
    efDeleter = unique_ptr<ExistingFileDeleter>();
    
    if ( pdfSettings->GetPreset() == PrintToPdf::PdfPreset::Manufacturing)
    {
        pwRetriever = unique_ptr<PasswordRetriever> { make_unique<NonePasswordRetriever>() };
    }
    else if (pdfSettings->GetPreset() == PrintToPdf::PdfPreset::Proof)
    {
        pwRetriever = unique_ptr<PasswordRetriever> { make_unique<ProofPasswordRetriever>() };
    }
    else if (pdfSettings->GetPreset() == PrintToPdf::PdfPreset::MicrProof)
    {
        pwRetriever = unique_ptr<PasswordRetriever> { make_unique<MicrPasswordRetriever>() };
    }
}

unique_ptr<PdfPrinter> PdfPrinter::GetPrinter(shared_ptr<PdfSettings> settings)
{
    if (settings->OutputSeparateFiles())
    {
        return unique_ptr<PdfPrinter> { make_unique<SeparateFilePdfPrinter>(SeparateFilePdfPrinter(move(settings))) };
    }
    else
    {
        return unique_ptr<PdfPrinter> { make_unique<SingleFilePdfPrinter>(SingleFilePdfPrinter(move(settings))) };
    }
}

const PlateNumber PdfPrinter::GetPlateNumber() const
{
    return gPlugin->sgJobFile->GetPlateNumber();
}

const string PdfPrinter::GetToken(int plateIndex) const
{
    return gPlugin->sgJobFile->GetToken(plateIndex);
}

PdfResults PdfPrinter::Print() const
{
    PdfResults transactions;
    
    if (pwRetriever->IsValid())
    {
        pdfSettings->SetPasswords(pwRetriever);
    }
    
    pdfSettings->SetBleeds(gPlugin->sgJobFile->GetBleeds(pdfSettings->GetPreset()));
    
    if ( pathCreator->CreatePath(outputPath) )
    {
        transactions.AddResult(efDeleter->Delete(GetPlateNumber(), outputPath));
        transactions.AddResult(CustomPrintSteps());
    }   
    
    return transactions;
}

PdfResults SingleFilePdfPrinter::CustomPrintSteps() const
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
    pdfSettings->SetPath(pathToPdfFile);
    
    // Set Range
    pdfSettings->SetVpbRange(string(pdfSettings->GetRange()));
        
    sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, *pdfSettings);
    
    transactions.AddResult({PdfResults::Transaction::Created, pathToPdfFile});
    
    return transactions;
}

PdfResults SeparateFilePdfPrinter::CustomPrintSteps() const
{
    PdfResults transactions;
    
    AIArtboardRangeIterator iter;
    sAIArtboardRange->Begin(pdfSettings->GetRange(), &iter);
    ai::int32 index = 0;
    
    ai::FilePath pathToPdfFile = outputPath;
    
    while ( kEndOfRangeErr != sAIArtboardRange->Next(iter, &index) ) {
        
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
        pdfSettings->SetPath(pathToPdfFile);
        
        // Set Range
        pdfSettings->SetVpbRange(to_string(index+1));
        
        sAIActionManager->PlayActionEvent(kSaveACopyAsAction, kDialogOff, *pdfSettings);
        
        transactions.AddResult({PdfResults::Transaction::Created, pathToPdfFile});
        
        pathToPdfFile.RemoveComponent();
    }
    
    sAIArtboardRange->DisposeIterator(iter);
    
    return transactions;
}
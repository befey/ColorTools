//
//  PdfPrinter.h
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#ifndef __SafeguardTools__PdfPrinter__
#define __SafeguardTools__PdfPrinter__

#include "PdfSettings.h"
#include "PdfResults.h"
#include "PasswordRetriever.h"
#include "PathBuilder.h"
#include "PathCreator.h"
#include "ExistingFileDeleter.h"
#include "AIFilePath.h"

namespace PrintToPdf
{
    class PdfPrinter
    {
    public:
        static unique_ptr<PdfPrinter> GetPrinter(shared_ptr<PdfSettings> settings);
        
        PdfResults Print() const;
        
    protected:
        PdfPrinter(shared_ptr<PdfSettings> settings);
        
        shared_ptr<PdfSettings> pdfSettings;
        unique_ptr<PasswordRetriever> pwRetriever;
        unique_ptr<PathBuilder> pathBuilder;
        unique_ptr<PathCreator> pathCreator;
        unique_ptr<ExistingFileDeleter> efDeleter;
        
        ai::FilePath outputPath;
        
        const PlateNumber GetPlateNumber() const;
        const string GetToken(int plateIndex) const;
        
    private:
        virtual PdfResults CustomPrintSteps() const = 0;
    };
    
    class SingleFilePdfPrinter : public PdfPrinter
    {
    public:
        SingleFilePdfPrinter(shared_ptr<PdfSettings> settings);

        PdfResults CustomPrintSteps() const override;
    };
    
    class SeparateFilePdfPrinter : public PdfPrinter
    {
    public:
        SeparateFilePdfPrinter(shared_ptr<PdfSettings> settings);

        PdfResults CustomPrintSteps() const;
    };
}
#endif /* defined(__SafeguardTools__PrintToPdf__) */

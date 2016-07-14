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
#include "LayerVisibility.hpp"
#include "TypeToPathsConverter.hpp"
#include "AIFilePath.h"

namespace PrintToPdf
{
    class PdfPrinter
    {
    public:
        static unique_ptr<PdfPrinter> GetPrinter(unique_ptr<PdfSettings> settings);
        
        PdfResults Print() const;
        
    protected:
        PdfPrinter(unique_ptr<PdfSettings> settings);
        
        unique_ptr<PdfSettings> pdfSettings;
        unique_ptr<PasswordRetriever> pwRetriever;
        unique_ptr<PathBuilder> pathBuilder;
        unique_ptr<PathCreator> pathCreator;
        unique_ptr<ExistingFileDeleter> efDeleter;
        unique_ptr<LayerVisibility> layerVisibility;
        unique_ptr<TypeToPathsConverter> tpConverter;
        
        ai::FilePath outputPath;
        
        const SafeguardFile::PlateNumber GetPlateNumber() const;
        const string GetToken(int plateIndex) const;
        
    private:
        virtual PdfResults CustomPrintSteps() const = 0;
    };
    
    class SingleFilePdfPrinter : public PdfPrinter
    {
    public:
        SingleFilePdfPrinter(unique_ptr<PdfSettings> settings);

        PdfResults CustomPrintSteps() const override;
    };
    
    class SeparateFilePdfPrinter : public PdfPrinter
    {
    public:
        SeparateFilePdfPrinter(unique_ptr<PdfSettings> settings);

        PdfResults CustomPrintSteps() const;
    };
}
#endif /* defined(__SafeguardTools__PrintToPdf__) */

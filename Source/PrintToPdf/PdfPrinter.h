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
#include "FilesystemResults.hpp"
#include "PathBuilder.h"
#include "ExistingFileDeleter.h"
#include "LayerVisibility.hpp"
#include "TypeToPathsConverter.hpp"
#include "PlateNumber.h"
#include "PrintToPdfCommand.hpp"
#include "FilesystemCommand.hpp"

namespace PrintToPdf
{
    class PdfPrinter
    {
    public:
        static FilesystemResults Print(const PdfSettings& settings);
        
    protected:
        PdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder);
        
        unique_ptr<PathBuilder> pathBuilder;
        unique_ptr<ExistingFileDeleter> efDeleter;
        unique_ptr<LayerVisibility> layerVisibility;
        unique_ptr<TypeToPathsConverter> tpConverter;
        
        ai::FilePath outputPath;
        
        SafeguardFile::PlateNumber plateNumber;
    private:
        PrintToPdfCommand printCommand;
        FilesystemResults transactions;
        
        static unique_ptr<PdfPrinter> GetPrinter(PdfPreset preset, const bool separateFiles, const bool doNotDelete, const bool userOutputFolder);
        
        FilesystemResults DoIt(const PdfSettings& settings);
        virtual FilesystemResults CustomPrintSteps(const PdfSettings& settings) const = 0;
    };
    
    class SingleFilePdfPrinter : public PdfPrinter
    {
    public:
        SingleFilePdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder);

        FilesystemResults CustomPrintSteps(const PdfSettings& settings) const override;
    };
    
    class SeparateFilePdfPrinter : public PdfPrinter
    {
    public:
        SeparateFilePdfPrinter(const PdfPreset preset, const bool doNotDelete, const bool userOutputFolder);

        FilesystemResults CustomPrintSteps(const PdfSettings& settings) const override;
    };
}
#endif /* defined(__SafeguardTools__PrintToPdf__) */

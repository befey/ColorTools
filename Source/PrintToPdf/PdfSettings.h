//
//  PdfSettings.h
//  SafeguardTools
//
//  Created by T431962 on 4/12/16.
//
//

#ifndef __SafeguardTools__PdfSettings__
#define __SafeguardTools__PdfSettings__

//=================================
// included dependencies
#include "AIActionManager.h"
#include "AIDocument.h"
#include "AIArtboard.h"
#include "BtArtboardRange.h"
#include "VPB.h"
#include "PlateNumber.h"
#include "PrintToPdfUIController.h"
#include "PdfResults.h"
#include "PasswordRetriever.h"
#include "SavePathCreator.h"
#include "PrintToPdfConstants.h"

//=================================
// forward declared dependencies
extern "C" AIActionManagerSuite* sAIActionManager;
extern "C" AIDocumentSuite* sAIDocument;
extern "C" AIArtboardSuite* sAIArtboard;

namespace PrintToPdf
{
    //=================================
    // PdfSettings - manages settings for PDF output
    class PdfSettings
    {
    public:
        PdfSettings(ai::FilePath sourceFile, PrintToPdf::PdfPreset p, unique_ptr<PasswordRetriever> pwRet, unique_ptr<SavePathCreator> spCre, string range = "", bool separateFiles = false);
        
        static PdfSettings MakePdfSettingsFromXml(const char* xmlData);
        
        PdfResults Print() const;
        
        void SetPreset(PrintToPdf::PdfPreset);
        void SetRange(string);
        
    private:
        BtArtboardRange range;
        bool separateFiles;
        PrintToPdf::PdfPreset preset;
        unique_ptr<PasswordRetriever> pwRetriever;
        unique_ptr<SavePathCreator> spCreator;
        
        VPB vpb;
        unique_ptr<PlateNumber> plateNumber;
        ai::FilePath outputPath;
        
        string CreateToken(int artboardIndex) const;
        AIRealRect CalculateBleeds();
    };
}

#endif /* defined(__SafeguardTools__PdfSettings__) */
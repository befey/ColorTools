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
#include "PrintToPdfConstants.h"
#include "PasswordRetriever.h"

//=================================
// forward declared dependencies
extern AIActionManagerSuite* sAIActionManager;
extern AIDocumentSuite* sAIDocument;
extern AIArtboardSuite* sAIArtboard;

namespace PrintToPdf
{
    //=================================
    // PdfSettings - manages settings for PDF output
    class PdfSettings
    {
    public:
        PdfSettings(PrintToPdf::PdfPreset p, string range = "", bool separateFiles = false);
        
        static unique_ptr<PdfSettings> MakePdfSettingsFromXml(const char* xmlData);
        
        void SetPreset(PrintToPdf::PdfPreset);
        PrintToPdf::PdfPreset GetPreset() const { return preset; };
        void SetRange(string);
        const BtArtboardRange& GetRange() const { return range; };
        void SetPasswords(const unique_ptr<PasswordRetriever> &pwRet);
        void SetBleeds(AIRealRect bleeds);
        void SetPath(ai::FilePath path);
        void SetVpbRange(string range);
        bool OutputSeparateFiles() const { return separateFiles; };
        
        inline operator AIActionParamValueRef(void) const { return AIActionParamValueRef(vpb); }
    private:
        BtArtboardRange range;
        bool separateFiles;
        PrintToPdf::PdfPreset preset;

        VPB vpb;
    };
}

#endif /* defined(__SafeguardTools__PdfSettings__) */
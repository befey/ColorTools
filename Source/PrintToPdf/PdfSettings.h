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
        
        static PdfSettings MakePdfSettingsFromXml(const char* xmlData);
        
        PrintToPdf::PdfPreset GetPreset() const { return preset; };
        const BtArtboardRange& GetRange() const { return range; };
        void SetBleeds(AIRealRect bleeds) const;
        void SetPath(ai::FilePath path) const;
        void SetVpbRange(string range) const;
        bool OutputSeparateFiles() const { return separateFiles; };
        
        inline operator AIActionParamValueRef(void) const { return AIActionParamValueRef(vpb); }
    private:
        BtArtboardRange range;
        bool separateFiles;
        PrintToPdf::PdfPreset preset;
        unique_ptr<PasswordRetriever> pwRetriever;

        VPB vpb;
        
        void SetPasswords();

    };
}

#endif /* defined(__SafeguardTools__PdfSettings__) */
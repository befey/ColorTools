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
#include "AIUser.h"
#include "BtArtboardRange.h"
#include "VPB.h"
#include "PlateNumber.h"
#include "PrintToPdfUIController.h"
#include "PrintToPdfConstants.h"
#include "PasswordRetriever.h"

//=================================
// forward declared dependencies
extern AIActionManagerSuite* sAIActionManager;
extern AIDocumentSuite* sAIDocument;
extern AIArtboardSuite* sAIArtboard;
extern AIUserSuite* sAIUser;

namespace PrintToPdf
{
    //=================================
    // PdfSettings - manages settings for PDF output
    class PdfSettings
    {
    public:
        PdfSettings(PrintToPdf::PdfPreset p, string range = "", bool separateFiles = false, bool doNotDelete = false, bool userOutputFolder = false);
        PdfSettings(PrintToPdf::PdfPreset p, AIReal bleedAmount, string range = "", bool separateFiles = false, bool doNotDelete = false, bool userOutputFolder = false);
        
        static PdfSettings MakePdfSettingsFromJson(const char* json);
        
        PrintToPdf::PdfPreset GetPreset() const { return preset; };
        const BtArtboardRange& GetRange() const { return range; };
        void SetBleeds(AIRealRect bleeds) const;
        void SetPath(ai::FilePath path) const;
        void SetVpbRange(string range) const;
        bool OutputSeparateFiles() const { return separateFiles; };
        bool DoNotDelete() const { return doNotDelete; };
        bool UserOutputFolder() const { return userOutputFolder; };
        
        inline operator AIActionParamValueRef(void) const { return AIActionParamValueRef(vpb); }
    private:
        BtArtboardRange range;
        bool separateFiles;
        bool doNotDelete;
        bool userOutputFolder;
        PrintToPdf::PdfPreset preset;
        shared_ptr<PasswordRetriever> pwRetriever;

        VPB vpb;
        
        void SetPasswords();
        static AIReal GetBleedAmountFromString(string expr);
    };
}

#endif /* defined(__SafeguardTools__PdfSettings__) */

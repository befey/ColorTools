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

//=================================
// forward declared dependencies
extern "C" AIActionManagerSuite* sAIActionManager;
extern "C" AIDocumentSuite* sAIDocument;
extern "C" AIArtboardSuite* sAIArtboard;


typedef void (*SettingsFunction) (AIActionParamValueRef);

//=================================
// PdfSettings - manages settings for PDF output
class PdfSettings
{
public:
    PdfSettings(SettingsFunction, string range = "", bool separateFiles = false);
    
    static PdfSettings MakePdfSettingsFromXml(const char* xmlData);
    
    void Print() const;
        
    void SetPreset(PrintToPdfUIController::PdfPreset);
    void SetRange(string);

    static SettingsFunction GetSettingsFuncForPdfPreset(PrintToPdfUIController::PdfPreset);
private:
    SettingsFunction settingsFunc;
    BtArtboardRange range;
    bool separateFiles;
    
    VPB vpb;
    PlateNumber* plateNumber;
    ai::FilePath outputPath;
    
    ai::FilePath CreateSaveFilePath();
    string CreateToken(int artboardIndex) const;
};

void ManufacturingSettingsFunc(AIActionParamValueRef);
void ProofSettingsFunc(AIActionParamValueRef target);
void MicrProofSettingsFunc(AIActionParamValueRef target);

#endif /* defined(__SafeguardTools__PdfSettings__) */
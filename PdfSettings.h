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

//=================================
// forward declared dependencies
extern "C" AIActionManagerSuite* sAIActionManager;
extern "C" AIDocumentSuite* sAIDocument;
extern "C" AIArtboardSuite* sAIArtboard;


//=================================
// Constant definitions
#define MANUFACTURING_PDF_PRESET    "Manufacturing"
#define MICR_PROOF_PDF_PRESET       "MICR"
#define REG_PROOF_PDF_PRESET        "Proof"

#define PATH_TO_PLANT_MANUFACTURING	"/Volumes/Plant_Manufacturing"
#define PATH_TO_PDFPROOFS "/Volumes/PDFProofs"
#define PATH_TO_MICR_PDF "/Volumes/MICR_PDF"
#define DEFAULT_OUTPUTPATH "/Users/t431962/Desktop/WORKING"

#define NO_TOKEN_DESIG "F"

typedef void (*SettingsFunction) (AIActionParamValueRef);

//=================================
// PdfSettings - manages settings for PDF output
class PdfSettings
{
public:
    PdfSettings(SettingsFunction, string range = "", bool separateFiles = false);
    
    void Print();
private:
    SettingsFunction settingsFunc;
    BtArtboardRange range;
    bool separateFiles;
    
    VPB vpb;
    PlateNumber* plateNumber;
    ai::FilePath outputPath;
    
    ai::FilePath CreateSaveFilePath();
    string CreateToken(int artboardIndex);
};

void ManufacturingSettingsFunc(AIActionParamValueRef);

#endif /* defined(__SafeguardTools__PdfSettings__) */
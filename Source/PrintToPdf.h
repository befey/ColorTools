//
//  PrintToPdf.h
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#ifndef __SafeguardTools__PrintToPdf__
#define __SafeguardTools__PrintToPdf__

#include "AIActionManager.h"
#include "VPB.h"
#include "PlateNumber.h"

#define PATH_TO_PLANT_MANUFACTURING	"/Volumes/Plant_Manufacturing"
#define PATH_TO_PDFPROOFS "/Volumes/PDFProofs"
#define PATH_TO_MICR_PDF "/Volumes/MICR_PDF"
#define DEFAULT_OUTPUTPATH "/Users/t431962/Desktop/WORKING"

#define MANUFACTURING_PDF_PRESET    "Manufacturing"
#define MICR_PROOF_PDF_PRESET       "MICR"
#define REG_PROOF_PDF_PRESET        "Proof"

extern "C" AIActionManagerSuite* sAIActionManager;

bool PrintToPdf();

void ManufacturingSettings(AIActionParamValueRef*);

ai::FilePath CreateSaveFilePath(int artboardIndex);

/** Gathers common parameters and saves a document.
 @param name IN name to save the document as.
 @param vpb IN the value parameter block.
 @return kNoErr on success, other error code otherwise.
 @see kAISaveDocumentAsAction
 @see SnpDocumentActionHelper::VPB
 */
ASErr SaveACopyAs(VPB&);


#endif /* defined(__SafeguardTools__PrintToPdf__) */

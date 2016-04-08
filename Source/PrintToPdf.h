//
//  PrintToPdf.h
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#ifndef __SafeguardTools__PrintToPdf__
#define __SafeguardTools__PrintToPdf__

#include "AIDocumentList.h"
#include "AIActionManager.h"
#include "VPB.h"
#include "PlateNumber.h"

#define PATH_TO_PLANT_MANUFACTURING	ai::UnicodeString("/Volumes/Plant_Manufacturing")
#define DEFAULT_OUTPUTPATH "/Users/t431962/Desktop/WORKING"

#define MANUFACTURING_PDF_PRESET    "Manufacturing"
#define MICR_PROOF_PDF_PRESET       "MICR"
#define REG_PROOF_PDF_PRESET        "Proof"

extern "C" AIDocumentListSuite* sAIDocumentList;
extern "C" AIActionManagerSuite* sAIActionManager;

bool PrintToPdf();

ai::FilePath GetManufacturingOutputFolder(PlateNumber);

/** Gathers action parameters and saves document as PDF.
 @param name IN name to save the document as.
 @return kNoErr on success, other error code otherwise.
 @see kAISaveDocumentAsAction
 */
ASErr SaveDocumentAsPDF(const string& name);

/** Gathers common parameters and saves a document.
 @param name IN name to save the document as.
 @param vpb IN the value parameter block.
 @return kNoErr on success, other error code otherwise.
 @see kAISaveDocumentAsAction
 @see SnpDocumentActionHelper::VPB
 */
ASErr SaveACopyAs(VPB& vpb);


/**	Sets the name key of the document to save as - kAISaveDocumentAsNameKey,
 in the value parameter block.
 @param name IN name to save document as.
 */
void SetSaveName(const ai::FilePath& name);


#endif /* defined(__SafeguardTools__PrintToPdf__) */

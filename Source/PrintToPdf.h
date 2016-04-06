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

#define PATH_TO_PLANT_MANUFACTURING	ai::UnicodeString("/Volumes/Plant_Manufacturing")
#define DEFAULT_OUTPUTPATH "/Users/t431962/Desktop/WORKING"

extern "C" AIDocumentListSuite* sAIDocumentList;
extern "C" AIActionManagerSuite* sAIActionManager;

bool PrintToPdf();

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
ASErr SaveACopyAs(const ai::UnicodeString& name, VPB& vpb);


#endif /* defined(__SafeguardTools__PrintToPdf__) */

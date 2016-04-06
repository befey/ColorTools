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
#include "VPB.h"

extern "C" AIDocumentListSuite* sAIDocumentList;

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

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

extern "C" AIActionManagerSuite* sAIActionManager;

bool PrintToPdf();

ai::FilePath CreateSaveFilePath(int artboardIndex);

ASErr SaveACopyAs(VPB&);

#endif /* defined(__SafeguardTools__PrintToPdf__) */

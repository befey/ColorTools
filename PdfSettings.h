//
//  PdfSettings.h
//  SafeguardTools
//
//  Created by T431962 on 4/12/16.
//
//

#ifndef __SafeguardTools__PdfSettings__
#define __SafeguardTools__PdfSettings__

#include "AIActionManager.h"
#include "VPB.h"

#define MANUFACTURING_PDF_PRESET    "Manufacturing"
#define MICR_PROOF_PDF_PRESET       "MICR"
#define REG_PROOF_PDF_PRESET        "Proof"

extern "C" AIActionManagerSuite* sAIActionManager;


class PdfSettings
{
public:
    static void Manufacturing(AIActionParamValueRef*);
    static void MicrProof(AIActionParamValueRef*);
    static void PdfProof(AIActionParamValueRef*);
};
#endif /* defined(__SafeguardTools__PdfSettings__) */
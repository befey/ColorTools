//
//  PrintToPdf.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#include "PrintToPdf.h"
#include "PdfSettings.h"
#include <string>

ASErr PrintToPdf()
{
    ASErr result = kNoErr;
    
    //TODO: Determine settings to use for PDF
    //Range? Bleed? Output location?
    
    //Create settings object for the PDF needed
    PdfSettings manufPdf(ManufacturingSettingsFunc, "1,3", true);

    //Output the PDF
    manufPdf.Print();
    
    return result;
}
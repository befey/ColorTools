//
//  PrintToPdf.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#include "PrintToPdf.h"
#include "PdfSettings.h"

ASErr PrintToPdf(const PdfSettings settings)
{
    ASErr result = kNoErr;

    //Output the PDF
    settings.Print();
    
    return result;
}
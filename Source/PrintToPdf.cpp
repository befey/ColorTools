//
//  PrintToPdf.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#include "PrintToPdf.h"
#include "PdfSettings.h"
#include "PdfResults.h"

PdfResults PrintToPdf(const PdfSettings settings)
{
    //Output the PDF
    PdfResults results = settings.Print();
    
    return results;
}
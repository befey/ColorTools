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
    PdfResults results;

    //Output the PDF
    settings.Print();
    
    return results;
}
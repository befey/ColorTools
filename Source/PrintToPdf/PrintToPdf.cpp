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

using PrintToPdf::PdfSettings;
using PrintToPdf::PdfResults;

PdfResults PrintToPdf::MakePdf(const PdfSettings settings)
{
    //Output the PDF
    PdfResults results = settings.Print();
    
    return results;
}
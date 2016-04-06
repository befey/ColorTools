//
//  PrintToPdf.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/4/16.
//
//

#include "PrintToPdf.h"
#include "VPB.h"

bool PrintToPdf()
{
    Instance instance;
    //We need to get the plate number from the plate struct
    ASErr error = instance.SaveDocumentAsPDF( "TESTPLATE#" );

    return true;
}
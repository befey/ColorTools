//
//  ExistingFileDeleter.h
//  SafeguardTools
//
//  Created by T431962 on 6/6/16.
//
//

#ifndef __SafeguardTools__ExistingFileDeleter__
#define __SafeguardTools__ExistingFileDeleter__

#include "PdfResults.h"
#include "PlateNumber.h"

namespace PrintToPdf
{
    class ExistingFileDeleter
    {
    public:
        static PdfResults Delete(SafeguardFile::PlateNumber, ai::FilePath);
    };
}
#endif /* defined(__SafeguardTools__ExistingFileDeleter__) */

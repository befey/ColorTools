//
//  SavePathCreator.h
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#ifndef __SafeguardTools__SavePathCreator__
#define __SafeguardTools__SavePathCreator__

#include "PrintToPdfConstants.h"
#include "PlateNumber.h"

namespace PrintToPdf
{
    class SavePathCreator
    {
    public:
        virtual ai::FilePath GetAiFilePath(PlateNumber&) = 0;
        
    };
    
    
    class ManufacturingSavePathCreator : public SavePathCreator
    {
    public:
        ai::FilePath GetAiFilePath(PlateNumber&);
    };
    
    
    class TestingSavePathCreator : public SavePathCreator
    {
    public:
        ai::FilePath GetAiFilePath(PlateNumber& pn);
    };
}
#endif /* defined(__SafeguardTools__SavePathCreator__) */


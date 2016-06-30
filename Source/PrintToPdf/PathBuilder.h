//
//  PathBuilder.h
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#ifndef __SafeguardTools__PathBuilder__
#define __SafeguardTools__PathBuilder__

#include "PrintToPdfConstants.h"
#include "PlateNumber.h"

namespace PrintToPdf
{
    class PathBuilder
    {
    public:
        virtual ai::FilePath GetAiFilePath(PlateNumber pn) = 0;
        
    };
    
    
    class ManufacturingPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(PlateNumber pn);
    };
    
    class ProofPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(PlateNumber pn);
    };
    
    class MicrProofPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(PlateNumber pn);
    };
 
    
    class TestingPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(PlateNumber pn);
    };
}
#endif /* defined(__SafeguardTools__PathBuilder__) */


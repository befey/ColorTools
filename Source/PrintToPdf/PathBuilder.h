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
#include "AIFilePath.h"

namespace PrintToPdf
{
    class PathBuilder
    {
    public:
        virtual ai::FilePath GetAiFilePath(SafeguardFile::PlateNumber pn) = 0;
        
    };
    
    
    class ManufacturingPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(SafeguardFile::PlateNumber pn);
    };
    
    class ProofPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(SafeguardFile::PlateNumber pn);
    };
    
    class MicrProofPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(SafeguardFile::PlateNumber pn);
    };
 
    
    class TestingPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(SafeguardFile::PlateNumber pn);
    };
}
#endif /* defined(__SafeguardTools__PathBuilder__) */


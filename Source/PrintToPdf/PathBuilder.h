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
#include "AIUser.h"

extern AIUserSuite* sAIUser;

namespace PrintToPdf
{
    class PathBuilder
    {
    public:
        virtual ai::FilePath GetAiFilePath(const SafeguardFile::PlateNumber pn) const = 0;
        
    };
    
    
    class ManufacturingPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(const SafeguardFile::PlateNumber pn) const override;
    };
    
    class ProofPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(const SafeguardFile::PlateNumber pn) const override;
    };
    
    class MicrProofPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(const SafeguardFile::PlateNumber pn) const override;
    };
 
    class UserPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(const SafeguardFile::PlateNumber pn) const override;
    };
    
    class TestingPathBuilder : public PathBuilder
    {
    public:
        ai::FilePath GetAiFilePath(const SafeguardFile::PlateNumber pn) const override;
    };
}
#endif /* defined(__SafeguardTools__PathBuilder__) */


//
//  PathCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#include "PathBuilder.h"
#include "PrintToPdfConstants.h"

using PrintToPdf::ManufacturingPathBuilder;
using PrintToPdf::ProofPathBuilder;
using PrintToPdf::MicrProofPathBuilder;
using PrintToPdf::TestingPathBuilder;

ai::FilePath ManufacturingPathBuilder::GetAiFilePath(PlateNumber pn)
{
    string prodCode;
    if (pn.GetProductType() == PlateNumber::BusinessStat)
    {
        prodCode = "BS";
    }
    else if (pn.GetProductIndicator() == "RC" || pn.GetProductIndicator() == "VS")
    {
        prodCode = "RC-VS";
    }
    else
    {
        prodCode = pn.GetProductIndicator();
    }
    
    ai::UnicodeString fpUS = ai::UnicodeString(PATH_TO_PLANT_MANUFACTURING);
    ai::FilePath saveasFilePath(fpUS);
    saveasFilePath.AddComponent(ai::UnicodeString(prodCode + " to be Plated"));
    saveasFilePath.AddComponent(ai::UnicodeString(pn.GetPlantIndicator() + " " + prodCode + " to be Plated"));
    
    return saveasFilePath;
}

ai::FilePath ProofPathBuilder::GetAiFilePath(PlateNumber pn)
{
    ai::UnicodeString fpUS = ai::UnicodeString(PATH_TO_PDFPROOFS);
    ai::FilePath saveasFilePath(fpUS);
    saveasFilePath.AddComponent(ai::UnicodeString("out"));
    
    return saveasFilePath;
}

ai::FilePath MicrProofPathBuilder::GetAiFilePath(PlateNumber pn)
{
    ai::UnicodeString fpUS = ai::UnicodeString(PATH_TO_MICR_PDF);
    ai::FilePath saveasFilePath(fpUS);
    saveasFilePath.AddComponent(ai::UnicodeString("out"));
    
    return saveasFilePath;
}


ai::FilePath TestingPathBuilder::GetAiFilePath(PlateNumber pn)
{
    string prodCode;
    if (pn.GetProductType() == PlateNumber::BusinessStat)
    {
        prodCode = "BS";
    }
    else if (pn.GetProductIndicator() == "RC" || pn.GetProductIndicator() == "VS")
    {
        prodCode = "RC-VS";
    }
    else
    {
        prodCode = pn.GetProductIndicator();
    }
        
    ai::UnicodeString fpUS = ai::UnicodeString(PrintToPdf::DEFAULT_OUTPUTPATH);
    ai::FilePath saveasFilePath(fpUS);
    saveasFilePath.AddComponent(ai::UnicodeString(prodCode + " to be Plated"));
    saveasFilePath.AddComponent(ai::UnicodeString(pn.GetPlantIndicator() + " " + prodCode + " to be Plated"));
        
    return saveasFilePath;
}
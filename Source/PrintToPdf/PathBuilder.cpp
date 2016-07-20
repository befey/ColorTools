//
//  PathCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#include "PathBuilder.h"
#include "PrintToPdfConstants.h"
#include "SafeguardFileConstants.h"

using SafeguardFile::PlateNumber;
using PrintToPdf::ManufacturingPathBuilder;
using PrintToPdf::ProofPathBuilder;
using PrintToPdf::MicrProofPathBuilder;
using PrintToPdf::TestingPathBuilder;
using PrintToPdf::UserPathBuilder;

ai::FilePath ManufacturingPathBuilder::GetAiFilePath(PlateNumber pn)
{
    string prodCode;
    ai::FilePath saveasFilePath;
    
    if (pn.GetProductType() == SafeguardFile::BusinessStat)
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
    
    if (prodCode == "")
    {
        UserPathBuilder upb;
        saveasFilePath = upb.GetAiFilePath(pn);
    }
    else
    {
        ai::UnicodeString fpUS = ai::UnicodeString(PATH_TO_PLANT_MANUFACTURING);
        saveasFilePath.Set(fpUS);
        saveasFilePath.AddComponent(ai::UnicodeString(prodCode + " to be Plated"));
        saveasFilePath.AddComponent(ai::UnicodeString(pn.GetPlantIndicator() + " " + prodCode + " to be Plated"));
    }
    
    return saveasFilePath;
}

ai::FilePath ProofPathBuilder::GetAiFilePath(PlateNumber pn)
{
    ai::UnicodeString fpUS = ai::UnicodeString(PATH_TO_PDFPROOFS);
    ai::FilePath saveasFilePath(fpUS);
    saveasFilePath.AddComponent(ai::UnicodeString("out"));
    
    if (saveasFilePath.Exists(true))
    {
        UserPathBuilder upb;
        saveasFilePath = upb.PathBuilder::GetAiFilePath(pn);
    }
    
    return saveasFilePath;
}

ai::FilePath MicrProofPathBuilder::GetAiFilePath(PlateNumber pn)
{
    ai::UnicodeString fpUS = ai::UnicodeString(PATH_TO_MICR_PDF);
    ai::FilePath saveasFilePath(fpUS);
    saveasFilePath.AddComponent(ai::UnicodeString("out"));
    
    if (saveasFilePath.Exists(true))
    {
        UserPathBuilder upb;
        saveasFilePath = upb.PathBuilder::GetAiFilePath(pn);
    }
    
    return saveasFilePath;
}

ai::FilePath UserPathBuilder::GetAiFilePath(PlateNumber pn)
{
    ai::FilePath saveasFilePath;
    sAIUser->GetDirectoryDialog(ai::UnicodeString("Choose a location to save the PDF files"), saveasFilePath);
    
    return saveasFilePath;
}


ai::FilePath TestingPathBuilder::GetAiFilePath(PlateNumber pn)
{
    string prodCode;
    ai::FilePath saveasFilePath;
    
    if (pn.GetProductType() == SafeguardFile::BusinessStat)
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
    
    if (prodCode == "")
    {
        UserPathBuilder upb;
        saveasFilePath = upb.GetAiFilePath(pn);
    }
    else
    {
        ai::UnicodeString fpUS = ai::UnicodeString(TESTING_OUTPUTPATH);
        saveasFilePath.Set(fpUS);
        saveasFilePath.AddComponent(ai::UnicodeString(prodCode + " to be Plated"));
        saveasFilePath.AddComponent(ai::UnicodeString(pn.GetPlantIndicator() + " " + prodCode + " to be Plated"));
    }
    
    return saveasFilePath;
}
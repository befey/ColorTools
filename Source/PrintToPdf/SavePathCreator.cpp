//
//  SavePathCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#include "SavePathCreator.h"
#include "PrintToPdfConstants.h"

using PrintToPdf::ManufacturingSavePathCreator;
using PrintToPdf::TestingSavePathCreator;

ai::FilePath ManufacturingSavePathCreator::GetAiFilePath(PlateNumber& pn)
{
    ai::UnicodeString fpUS = ai::UnicodeString(PATH_TO_PLANT_MANUFACTURING);
    ai::FilePath saveasFilePath(fpUS);
    saveasFilePath.AddComponent(ai::UnicodeString(pn.GetProductIndicator() + " to be Plated"));
    saveasFilePath.AddComponent(ai::UnicodeString(pn.GetPlantIndicator() + " " + pn.GetProductIndicator() + " to be Plated"));
    return saveasFilePath;
}

ai::FilePath TestingSavePathCreator::GetAiFilePath(PlateNumber& pn)
{
    ai::UnicodeString fpUS = ai::UnicodeString(PrintToPdf::DEFAULT_OUTPUTPATH);
    ai::FilePath saveasFilePath(fpUS);
    saveasFilePath.AddComponent(ai::UnicodeString(pn.GetProductIndicator() + " to be Plated"));
    saveasFilePath.AddComponent(ai::UnicodeString(pn.GetPlantIndicator() + " " + pn.GetProductIndicator() + " to be Plated"));
    return saveasFilePath;
}
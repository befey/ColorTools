//
//  SavePathCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/2/16.
//
//

#include "SavePathCreator.h"
#include "PrintToPdfConstants.h"
#include <boost/system/system_error.hpp>
#include <boost/filesystem.hpp>

using PrintToPdf::ManufacturingSavePathCreator;
using PrintToPdf::TestingSavePathCreator;
namespace fs = boost::filesystem;

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
    
    fs::path p = saveasFilePath.GetFullPath().as_Platform();
    
    boost::system::error_code e;
    fs::create_directories(p, e);
    
    return saveasFilePath;
}
//
//  PlateNumber.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/7/16.
//
//

#include "PlateNumber.h"
#include <regex>

using SafeguardFile::PlateNumber;
using SafeguardFile::ProductType;

PlateNumber::PlateNumber(std::string pNum)
{
    for (auto &c : pNum) c = toupper(c);
    plateNumber = pNum;
    
    isValidPlateNumber = TokenizePlateNumber();
    
    if (IsValid() && plantIndicator.length() == 0)
    {
        plantIndicator = productIndicator[0];
    }
}

bool PlateNumber::TokenizePlateNumber()
{
    std::regex r("(?:^(?:([a-z])(\\d{2}))?(([a-z]{2})(\\d{3,6})){1}?[.]?(\\S*))", std::regex::icase);
    
    std::smatch result;
    
    std::regex_search(plateNumber, result, r);
    
    /*There's two different plate number formats -- Y16SF000123 and SF00123 if we have a plate number in one of those formats we will get a result with matches:
        0: <the input plate number>
        1: <the plant indicator if the first type>
        2: <the year if the first type>
        3: <UNUSED> group 4 and 5 together
        4: <the product indicator>
        5: <the number>
        6: any trailing .BP, etc. but without the "."
    */

    if (result.size() == 0)
    {
        return false;
    }
    
    else if (result[1].matched)
    {
        plantIndicator = result[1];
        year = result[2];
    }
    productIndicator = result[4];
    number = result[5];
    suffix = result[6];
    return true;
}

ProductType PlateNumber::GetProductType() const
{
    if (!IsValid())
    {
        return ProductType::INVAL;
    }
    
    if (productIndicator.at(0) == 'B' || productIndicator == "MP" || productIndicator == "SP")
    {
        return ProductType::BusinessStat;
    }
    
    if (productIndicator == "SF")
    {
        return ProductType::CutSheet;
    }
    
    if (productIndicator == "CF" || productIndicator == "SC" || productIndicator == "SS")
    {
        return ProductType::Continuous;
    }
    
    if (productIndicator == "EN")
    {
        return ProductType::Envelope;
    }
    
    if (productIndicator == "CS")
    {
        if (HasInnerTicks())
        {
            return ProductType::Snapset;
        }
        else
        {
            return ProductType::CutSheet;
        }
    }
    
    //OTHERS
    if (productIndicator == "AR" || productIndicator == "CK" || productIndicator == "CM" || productIndicator == "GC" || productIndicator == "LS" || productIndicator == "QC" || productIndicator == "RC" || productIndicator == "VP")
    {
        return ProductType::Snapset;
    }
    
    return ProductType::INVAL;
}

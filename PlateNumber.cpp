//
//  PlateNumber.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/7/16.
//
//

#include "PlateNumber.h"
#include <regex>

PlateNumber::PlateNumber(string pNum)
{
    for (auto & c: pNum) c = toupper(c);
    plateNumber = pNum;
    
    isValidPlateNumber = TokenizePlateNumber();
    
    if (IsValid() && plantIndicator.length() == 0) {
        plantIndicator = productIndicator[0];
    }
}


Boolean PlateNumber::TokenizePlateNumber()
{
    using namespace std;
    
    regex r("(?:^(?:([a-z])(\\d{2}))?([a-z]{2})(\\d{3,6}$))", regex::icase);
    
    smatch result;
    regex_search(plateNumber, result, r);
    
    /*There's two different plate number formats -- Y16SF000123 and SF00123 if we have a plate number in one of those formats we will get a result with matches:
        0: <the input plate number>
        1: <the plant indicator if the first type>
        2: <the year if the first type>
        3: <the product indicator>
        4: <the number>
    */
    
    if (result.size() == 0) {
        return false;
    }
    
    else if (result[1].matched)
    {
        plantIndicator = result[1];
        year = result[2];
    }
    productIndicator = result[3];
    number = result[4];
    return true;
}
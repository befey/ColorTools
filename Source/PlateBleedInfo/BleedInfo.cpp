//
//  BleedInfo.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/26/16.
//
//

#include "BleedInfo.h"

using SafeguardFile::BleedInfo;

int BleedInfo::GetTickMarkOffset()
{
    ProductType pt = plateNumber.GetProductType();
    
    if (pt == ProductType::CutSheet)
    {
        return 27;
    }
    else if (pt == ProductType::Snapset || pt == ProductType::Continuous)
    {
        return 9;
    }
}
//
//  TickMarkSettings.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/26/16.
//
//

#include "TickMarkSettings.hpp"
#include "ColorFuncs.h"

using SafeguardFile::TickMarkSettings;
using SafeguardFile::BleedInfo;

TickMarkSettings::TickMarkSettings(BleedInfo bleedInfo)
{
    bounds = bleedInfo.rect;
    offset = bleedInfo.GetTickMarkOffset();

    ProductType pt = bleedInfo.plateNumber.GetProductType();
    if (pt == ProductType::Continuous || pt == ProductType::Snapset)
    {
        drawInner = true;
    }
    if (pt == ProductType::Continuous || pt == ProductType::CutSheet)
    {
        drawOuter = true;
    }
    
    color = GetRegistrationColor();
    if (pt == ProductType::CutSheet)
    {
        color.c.c.tint = .8;
    }
}
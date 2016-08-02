//
//  BleedInfo.h
//  SafeguardTools
//
//  Created by T431962 on 6/7/16.
//
//

#ifndef __SafeguardTools__BleedInfo__
#define __SafeguardTools__BleedInfo__

#include "AIUser.h"
#include "ColorList.h"
#include "PlateNumber.h"
#include <ctime>

namespace SafeguardFile
{
    class BleedInfo
    {
    public:
        int GetTickMarkOffset() const;

        ai::ArtboardID artboardIndex;
        AIRealRect rect;
        ColorList colorList;
        PlateNumber plateNumber;
        string token;
        tm lastModified;;
        bool ShouldAddCMYKBlocks;
    };
}

#endif /* defined(__SafeguardTools__BleedInfo__) */

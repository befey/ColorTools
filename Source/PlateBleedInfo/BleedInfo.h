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
#include "TickMarkDrawer.h"
#include "ColorListDrawer.h"
#include "FileNameDateDrawer.h"
#include <ctime>

namespace SafeguardFile
{
    struct BleedInfo
    {
        ai::ArtboardID artboardIndex;
        AIRealRect rect;
        ColorList colorList;
        PlateNumber plateNumber;
        string token;
        tm lastModified;
        shared_ptr<SafeguardFile::TickMarkDrawer> tickMarkDrawer;
        shared_ptr<SafeguardFile::ColorListDrawer> colorListDrawer;
        shared_ptr<SafeguardFile::FileNameDateDrawer> fileNameDateDrawer;
        bool ShouldAddCMYKBlocks;
    };
}

#endif /* defined(__SafeguardTools__BleedInfo__) */

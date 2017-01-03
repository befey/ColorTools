//
//  BleedInfoDrawableController.h
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#ifndef __SafeguardTools__BleedInfoDrawableController__
#define __SafeguardTools__BleedInfoDrawableController__

#include "BleedInfo.h"
#include "TickMarkDrawer.h"
#include "ColorListDrawer.h"
#include "FileNameDateDrawer.h"
#include "SgSymbolDrawer.hpp"
#include "AIArt.h"
#include "AIPluginGroup.h"
#include "IDrawer.h"
#include "IDrawable.hpp"
#include <ctime>

extern AIArtSuite* sAIArt;
extern AIPluginGroupSuite* sAIPluginGroup;

namespace PlateBleedInfo
{
    constexpr auto PLATE_BLEED_INFO_GROUP_LABEL =             "__plate_bleed_info__";
    
    class BleedInfoDrawableController
    {
    public:
        BleedInfoDrawableController(shared_ptr<BleedInfo> bleedInfo);
        
        AIArtHandle Draw() const;
        AIArtHandle Remove() const;
              
    private:
        shared_ptr<BleedInfo> bleedInfo;
        vector<shared_ptr<IDrawable>> drawables;
                        
        AIArtHandle Add() const;
        AIArtHandle Update(AIArtHandle pluginGroupArt) const;
        AIArtHandle CreateResultArt(AIArtHandle pluginGroupArt) const;
        void ClearResultArt(AIArtHandle resultGroupArt) const;
    };
}

#endif /* defined(__SafeguardTools__BleedInfoDrawableController__) */

//
//  BleedInfoDrawer.h
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#ifndef __SafeguardTools__BleedInfoDrawer__
#define __SafeguardTools__BleedInfoDrawer__

#include "BleedInfo.h"
#include "TickMarkDrawer.h"
#include "ColorListDrawer.h"
#include "FileNameDateDrawer.h"
#include "AIArt.h"
#include "AIPluginGroup.h"
#include "IDrawer.h"

extern AIArtSuite* sAIArt;
extern AIPluginGroupSuite* sAIPluginGroup;

namespace SafeguardFile
{
    constexpr auto PLATE_BLEED_INFO_GROUP_LABEL =             "__plate_bleed_info__";
    
    class BleedInfoDrawer : public IDrawer
    {
    public:
        BleedInfoDrawer(ai::ArtboardID artboardIndex) : artboardIndex(artboardIndex) {};
        
        BleedInfoDrawer& AddDrawer(shared_ptr<IDrawer> val) { drawers.push_back(val); return *this; };

        void Remove(AIArtHandle& pluginGroupArt) const;
    private:
        ai::ArtboardID artboardIndex;
        
        vector<shared_ptr<IDrawer>> drawers;
        
        AIArtHandle DoDraw(AIArtHandle pluginGroupArt = NULL) const override;
        
        AIArtHandle Add() const;
        AIArtHandle Update(AIArtHandle pluginGroupArt) const;
        AIArtHandle CreateResultArt(AIArtHandle pluginGroupArt) const;
        void ClearResultArt(AIArtHandle resultGroupArt) const;
    };
}

#endif /* defined(__SafeguardTools__BleedInfoDrawer__) */

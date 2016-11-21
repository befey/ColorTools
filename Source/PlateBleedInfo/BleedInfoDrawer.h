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
#include "SgSymbolDrawer.hpp"
#include "AIArt.h"
#include "AIPluginGroup.h"
#include "IDrawer.h"
#include <ctime>

extern AIArtSuite* sAIArt;
extern AIPluginGroupSuite* sAIPluginGroup;

namespace PlateBleedInfo
{
    constexpr auto PLATE_BLEED_INFO_GROUP_LABEL =             "__plate_bleed_info__";
    
    class BleedInfoDrawer : public IDrawer
    {
    public:
        BleedInfoDrawer(BleedInfo bleedInfo);
        
        AIArtHandle Remove() const;
    private:
        BleedInfo bleedInfo;
        
        vector<shared_ptr<IDrawer>> drawers;
        
        BleedInfoDrawer& AddDrawer(shared_ptr<IDrawer> val);
        shared_ptr<IDrawer> MakeColorListDrawer(SafeguardFile::ProductType pt, AIRealRect artboardBounds, SafeguardFile::ColorList colorList);
        shared_ptr<IDrawer> MakeFileNameDateDrawer(SafeguardFile::ProductType pt, AIRealRect artboardBounds, SafeguardFile::PlateNumber plateNumber, string token, tm lastModified);
        shared_ptr<IDrawer> MakeTickMarkDrawer(SafeguardFile::TickMarkSettings tmSettings);
        shared_ptr<IDrawer> MakeSgSymbolDrawer(AIRealRect artboardBounds, string symbolName);
        
        AIArtHandle DoDraw(AIArtHandle pluginGroupArt = NULL) const override;
        
        AIArtHandle Add() const;
        AIArtHandle Update(AIArtHandle pluginGroupArt) const;
        AIArtHandle CreateResultArt(AIArtHandle pluginGroupArt) const;
        void ClearResultArt(AIArtHandle resultGroupArt) const;
    };
}

#endif /* defined(__SafeguardTools__BleedInfoDrawer__) */

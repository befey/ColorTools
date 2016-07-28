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

extern AIArtSuite* sAIArt;
extern AIPluginGroupSuite* sAIPluginGroup;

namespace SafeguardFile
{
    constexpr auto PLATE_BLEED_INFO_GROUP_LABEL =             "__plate_bleed_info__";
    
    class BleedInfoDrawer
    {
    public:
        BleedInfoDrawer(shared_ptr<BleedInfo> info) : p_BleedInfo(info) {};
        
        void Draw() const;
        void Remove(AIArtHandle pluginGroupArt) const;
        
        BleedInfoDrawer& TickMarkDrawer(shared_ptr<SafeguardFile::TickMarkDrawer> val) { tickMarkDrawer = val; return *this; };
        BleedInfoDrawer& ColorListDrawer(shared_ptr<SafeguardFile::ColorListDrawer> val) { colorListDrawer = val; return *this; };
        BleedInfoDrawer& FileNameDateDrawer(shared_ptr<SafeguardFile::FileNameDateDrawer> val) { fileNameDateDrawer = val; return *this; };

    private:
        shared_ptr<BleedInfo> p_BleedInfo;
        
        shared_ptr<SafeguardFile::TickMarkDrawer> tickMarkDrawer;
        shared_ptr<SafeguardFile::ColorListDrawer> colorListDrawer;
        shared_ptr<SafeguardFile::FileNameDateDrawer> fileNameDateDrawer;
        
        void Add() const;
        void Update(AIArtHandle pluginGroupArt) const;
        void CreateResultArt(AIArtHandle pluginGroupArt) const;
        
        //ASErr PluginGroupNotify(AIPluginGroupMessage* message);
        //ASErr PluginGroupUpdate(AIPluginGroupMessage* message);
    };
}

#endif /* defined(__SafeguardTools__BleedInfoDrawer__) */

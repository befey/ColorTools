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

    private:
        shared_ptr<BleedInfo> p_BleedInfo;
        
        void Add() const;
        void Update(AIArtHandle pluginGroupArt) const;
        void CreateResultArt(AIArtHandle pluginGroupArt) const;
        
        //ASErr PluginGroupNotify(AIPluginGroupMessage* message);
        //ASErr PluginGroupUpdate(AIPluginGroupMessage* message);
    };
}

#endif /* defined(__SafeguardTools__BleedInfoDrawer__) */

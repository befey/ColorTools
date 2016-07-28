//
//  TickMarkDrawer.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__TickMarkDrawer__
#define __SafeguardTools__TickMarkDrawer__

#include "TickMarkSettings.hpp"
#include "AIArt.h"
#include "AIPath.h"
#include "AIPathStyle.h"

extern AIArtSuite* sAIArt;
extern AIPathSuite* sAIPath;
extern AIPathStyleSuite* sAIPathStyle;

namespace SafeguardFile
{
    class TickMarkDrawer
    {
    public:
        TickMarkDrawer(TickMarkSettings settings);
        
        AIArtHandle Draw();
        
    private:
        TickMarkSettings settings;
        
        AIArtHandle DrawInner(AIArtHandle tickMarkGroupArt = NULL);
        AIArtHandle DrawOuter(AIArtHandle tickMarkGroupArt = NULL);
    };
}

#endif /* defined(__SafeguardTools__TickMarkDrawer__) */
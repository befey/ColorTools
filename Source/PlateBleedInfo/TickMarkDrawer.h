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
#include "IDrawer.h"

extern AIArtSuite* sAIArt;
extern AIPathSuite* sAIPath;
extern AIPathStyleSuite* sAIPathStyle;

namespace SafeguardFile
{
    class TickMarkDrawer : public IDrawer
    {
    public:
        TickMarkDrawer(TickMarkSettings settings);
        
    private:
        struct TickMark
        {
            AIRealPoint start, mid, end;
        };
        
        TickMarkSettings settings;
        
        AIArtHandle DoDraw(AIArtHandle resultGroup) const override;
        AIArtHandle DrawTickMarks(vector<TickMark> ticks, AIArtHandle tickMarkGroupArt = NULL) const;
        AIArtHandle DrawInvisiblePath(AIArtHandle resultGroup, AIArtHandle tickMarkGroupArt = NULL) const;
        AIArtHandle DrawInner(AIArtHandle resultGroup, AIArtHandle tickMarkGroupArt = NULL) const;
        AIArtHandle DrawOuter(AIArtHandle resultGroup, AIArtHandle tickMarkGroupArt = NULL) const;
    };
}

#endif /* defined(__SafeguardTools__TickMarkDrawer__) */

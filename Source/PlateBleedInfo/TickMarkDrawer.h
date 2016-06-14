//
//  TickMarkDrawer.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__TickMarkDrawer__
#define __SafeguardTools__TickMarkDrawer__

#include <vector>

namespace PlateBleedInfo
{
    class TickMarkDrawer
    {
        virtual vector<AIArtHandle> DrawTickMarks() = 0;
    };
    
    class OuterTickMarkDrawer : public TickMarkDrawer
    {
        vector<AIArtHandle> DrawTickMarks();
    };
    
    class InnerTickMarkDrawer : public TickMarkDrawer
    {
        vector<AIArtHandle> DrawTickMarks();
    };
    
    class ContinuousTickMarkDrawer : public TickMarkDrawer
    {
        vector<AIArtHandle> DrawTickMarks();
    };
}

#endif /* defined(__SafeguardTools__TickMarkDrawer__) */
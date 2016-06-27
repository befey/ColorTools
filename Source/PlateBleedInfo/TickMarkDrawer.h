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

namespace SafeguardFile
{
    class BleedInfo;
    
    class TickMarkDrawer
    {
    public:
        virtual AIArtHandle Draw() = 0;
        
    protected:
        TickMarkDrawer(shared_ptr<BleedInfo> info);
        
        shared_ptr<BleedInfo> p_BleedInfo;
    };
    
    class OuterTickMarkDrawer : public TickMarkDrawer
    {
    public:
        OuterTickMarkDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
    
    class InnerTickMarkDrawer : public TickMarkDrawer
    {
    public:
        InnerTickMarkDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
    
    class ContinuousTickMarkDrawer : public TickMarkDrawer
    {
    public:
        ContinuousTickMarkDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
}

#endif /* defined(__SafeguardTools__TickMarkDrawer__) */
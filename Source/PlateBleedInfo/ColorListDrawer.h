//
//  ColorListDrawer.h
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#ifndef __SafeguardTools__ColorListDrawer__
#define __SafeguardTools__ColorListDrawer__

#include "BleedTextInfoDrawer.h"

namespace SafeguardFile
{
    class BleedInfo;
    
    class ColorListDrawer : public BleedTextInfoDrawer
    {
    protected:
        ColorListDrawer(shared_ptr<BleedInfo> info);
    };
    
    class LaserColorListDrawer : public ColorListDrawer
    {
    public:
        LaserColorListDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
    
    class ContinuousColorListDrawer : public ColorListDrawer
    {
    public:
        ContinuousColorListDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
    
    class BusStatColorListDrawer : public ColorListDrawer
    {
    public:
        BusStatColorListDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
}


#endif /* defined(__SafeguardTools__ColorListDrawer__) */
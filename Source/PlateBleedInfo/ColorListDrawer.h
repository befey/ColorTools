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
#include "ColorList.h"

namespace SafeguardFile
{
    class BleedInfo;
    
    class ColorListDrawer : public BleedTextInfoDrawer
    {
    protected:
        ColorListDrawer(AIRealRect bounds, AIRealPoint anchor, ColorList colorList);
        
        ColorList colorList;
        AIReal maxWidth;
    };
    
    class LaserColorListDrawer : public ColorListDrawer
    {
    public:
        LaserColorListDrawer(AIRealRect bounds, ColorList colorList);
    private:
        AIArtHandle DoDraw(AIArtHandle resultGroup) const override;
    };
    
    class ContinuousColorListDrawer : public ColorListDrawer
    {
    public:
        ContinuousColorListDrawer(AIRealRect bounds, ColorList colorList);
    private:
        AIArtHandle DoDraw(AIArtHandle resultGroup) const override;
    };
    
    class BusStatColorListDrawer : public ColorListDrawer
    {
    public:
        BusStatColorListDrawer(AIRealRect bounds, ColorList colorList);
    private:
        AIArtHandle DoDraw(AIArtHandle resultGroup) const override;
    };
}


#endif /* defined(__SafeguardTools__ColorListDrawer__) */

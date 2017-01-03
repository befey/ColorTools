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
#include "IDrawer.h"
#include "IDrawable.hpp"

struct ColorListDrawerSettings
{
    ColorListDrawerSettings(SafeguardFile::ProductType pt, AIRealRect artboardBounds, ColorList colorList) :
    pt(pt),
    artboardBounds(artboardBounds),
    colorList(colorList) {};
    
    SafeguardFile::ProductType pt;
    AIRealRect artboardBounds;
    ColorList colorList;
};


namespace SafeguardFile
{
    class BleedInfo;
    
    class ColorListDrawer : public BleedTextInfoDrawer
    {
    protected:
        ColorListDrawer(AIRealRect bounds, AIRealPoint anchor, ColorList colorList);
        
        ColorList colorList;
        AIReal maxWidth;
        AIReal maxHeight;
    };
    
    class LaserColorListDrawer : public ColorListDrawer
    {
    public:
        LaserColorListDrawer(AIRealRect bounds, ColorList colorList);
    private:
        AIArtHandle Draw(AIArtHandle resultGroup) const override;
    };
    
    class ContinuousColorListDrawer : public ColorListDrawer
    {
    public:
        ContinuousColorListDrawer(AIRealRect bounds, ColorList colorList);
    private:
        AIArtHandle Draw(AIArtHandle resultGroup) const override;
        void DrawContinuousColorBlocks(AIArtHandle resultGroup) const;
    };
    
    class BusStatColorListDrawer : public ColorListDrawer
    {
    public:
        BusStatColorListDrawer(AIRealRect bounds, ColorList colorList);
    private:
        AIArtHandle Draw(AIArtHandle resultGroup) const override;
    };
    
    class ColorListDrawable : public IDrawable
    {
    public:
        ColorListDrawable(ColorListDrawerSettings settings)
        {
            drawer = DrawerFactory().GetDrawer(settings);
        }
    };
}

template <>
class DrawerFactoryImpl<ColorListDrawerSettings>
{
public:
    static shared_ptr<IDrawer> GetDrawer(ColorListDrawerSettings settings)
    {
        if (settings.pt == SafeguardFile::ProductType::BusinessStat)
        {
            return make_shared<SafeguardFile::BusStatColorListDrawer>(settings.artboardBounds, settings.colorList);
        }
        else if (settings.pt == SafeguardFile::ProductType::Continuous)
        {
            return make_shared<SafeguardFile::ContinuousColorListDrawer>(settings.artboardBounds, settings.colorList);
        }
        else if (settings.pt == SafeguardFile::ProductType::CutSheet || settings.pt == SafeguardFile::ProductType::Envelope)
        {
            return make_shared<SafeguardFile::LaserColorListDrawer>(settings.artboardBounds, settings.colorList);
        }
        
        return nullptr;
    };
};

#endif /* defined(__SafeguardTools__ColorListDrawer__) */

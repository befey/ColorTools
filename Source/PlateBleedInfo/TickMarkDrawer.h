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
#include "IDrawable.hpp"

extern AIArtSuite* sAIArt;
extern AIPathSuite* sAIPath;
extern AIPathStyleSuite* sAIPathStyle;

constexpr auto TICKMARK_ARTHANDLE =            "__bt_tickmark_arthandle__";

namespace SafeguardFile
{
    class TickMarkDrawer : public IDrawer
    {
    public:
        TickMarkDrawer(TickMarkSettings settings);
        AIArtHandle Draw(AIArtHandle resultArt) const override;
        string GetDictionaryLabel(AIArtHandle resultArt) const override { return TICKMARK_ARTHANDLE + DictionaryWriter::GetUIDStringForArt(resultArt); };
        
    protected:
        TickMarkSettings settings;
        
        AIArtHandle DrawInvisiblePath(AIArtHandle resultGroup, AIArtHandle tickMarkGroupArt = nullptr) const;
        
    private:
        struct TickMark
        {
            AIRealPoint start, mid, end;
        };

        AIArtHandle DrawTickMarks(vector<TickMark> ticks, AIArtHandle tickMarkGroupArt = nullptr) const;
        AIArtHandle DrawInner(AIArtHandle resultGroup, AIArtHandle tickMarkGroupArt = nullptr) const;
        AIArtHandle DrawOuter(AIArtHandle resultGroup, AIArtHandle tickMarkGroupArt = nullptr) const;
    };
    
    class BsTickMarkDrawer : public TickMarkDrawer
    {
    public:
        BsTickMarkDrawer(TickMarkSettings settings) : TickMarkDrawer(settings) {};
        AIArtHandle Draw(AIArtHandle resultGroup) const override;
    };

    class NoneTickMarkDrawer : public TickMarkDrawer
    {
    public:
        NoneTickMarkDrawer(TickMarkSettings settings) : TickMarkDrawer(settings) {};
        AIArtHandle Draw(AIArtHandle resultGroup) const override { return DrawInvisiblePath(resultGroup); };
    };
    
    class TickMarkDrawable : public IDrawable
    {
    public:
        TickMarkDrawable(TickMarkSettings settings)
        {
            drawer = DrawerFactory().GetDrawer(settings);
        }
    };
}

using SafeguardFile::TickMarkDrawable;
using SafeguardFile::TickMarkSettings;
template <>
class DrawableFactoryImpl<TickMarkSettings>
{
public:
    static std::shared_ptr<IDrawable> GetDrawable(TickMarkSettings settings, AIArtHandle pluginArt)
    {
        if (true)
        {
            return make_shared<TickMarkDrawable>(settings);
        }
        return nullptr;
    };
};

using SafeguardFile::TickMarkSettings;
using SafeguardFile::TickMarkDrawer;
using SafeguardFile::BsTickMarkDrawer;
using SafeguardFile::NoneTickMarkDrawer;
template <>
class DrawerFactoryImpl<TickMarkSettings>
{
public:
    static shared_ptr<IDrawer> GetDrawer(TickMarkSettings tmSettings)
    {
        if (!tmSettings.ShouldDrawBleedInfo())
        {
            return make_shared<NoneTickMarkDrawer>(tmSettings);
        }
        
        if (tmSettings.TickMarkStyle() == SafeguardFile::TickMarkStyle::BsStyle)
        {
            return make_shared<BsTickMarkDrawer>(tmSettings);
        }
        else
        {
            return make_shared<TickMarkDrawer>(tmSettings);
        }
    };
};

#endif /* defined(__SafeguardTools__TickMarkDrawer__) */

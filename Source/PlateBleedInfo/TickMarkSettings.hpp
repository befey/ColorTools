//
//  TickMarkSettings.hpp
//  SafeguardTools
//
//  Created by T431962 on 7/26/16.
//
//

#ifndef TickMarkSettings_hpp
#define TickMarkSettings_hpp

#include "AIColor.h"
#include "SafeguardFileConstants.h"

namespace SafeguardFile
{  
    class TickMarkSettings
    {
    public:
        TickMarkSettings();
        TickMarkSettings(const AIRealRect rect, const ProductType pt, const SafeguardFile::TickMarkStyle tms, const bool shouldDrawBleedInfo);
        
        AIRealRect Bounds() const { return bounds; };
        int Offset() const { return offset; };
        float Weight() const { return weight; };
        AIColor Color() const { return color; };
        bool DrawInner() const;
        bool DrawOuter() const;
        bool ShouldDrawBleedInfo() const { return shouldDrawBleedInfo; };
        
        SafeguardFile::TickMarkStyle TickMarkStyle() const { return tmStyle; };
        TickMarkSettings& TickMarkStyle(SafeguardFile::TickMarkStyle newVal) { tmStyle = newVal; return *this;};
    private:
        static constexpr auto TICK_LENGTH_CONTINUOUS = 9;
        static constexpr auto TICK_LENGTH_NONE = 0;
        static constexpr auto TICK_STROKE_STANDARD = .5;
        
        static constexpr auto TICK_LENGTH_CUTSHEET = 9;
        static constexpr auto TICK_TINT_CUTSHEET = 0;
        static constexpr auto TICK_STROKE_CUTSHEET = .25;
        
        AIRealRect bounds;
        int offset;
        float weight = TICK_STROKE_STANDARD;
        AIColor color;
        SafeguardFile::TickMarkStyle tmStyle;
        bool shouldDrawBleedInfo;
    };
}

#endif /* TickMarkSettings_hpp */

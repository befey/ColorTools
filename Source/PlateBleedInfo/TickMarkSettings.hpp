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
        TickMarkSettings(const AIRealRect rect, const ProductType pt, const SafeguardFile::TickMarkStyle tms);
        
        AIRealRect Bounds() const { return bounds; };
        int Offset() const { return offset; };
        float Weight() const { return weight; };
        AIColor Color() const { return color; };
        bool DrawInner() const;
        bool DrawOuter() const;
        
        SafeguardFile::TickMarkStyle TickMarkStyle() const { return tmStyle; };
        TickMarkSettings& TickMarkStyle(SafeguardFile::TickMarkStyle newVal) { tmStyle = newVal; };
    private:
        AIRealRect bounds;
        int offset;
        float weight = .5;
        AIColor color;
        SafeguardFile::TickMarkStyle tmStyle;
        
        static constexpr auto TICK_LENGTH_CUTSHEET = 27;
        static constexpr auto TICK_LENGTH_CONTINUOUS = 9;
        static constexpr auto TICK_LENGTH_NONE = 0;
        
        static constexpr auto TICK_TINT_CUTSHEET = .8;
    };
}

#endif /* TickMarkSettings_hpp */
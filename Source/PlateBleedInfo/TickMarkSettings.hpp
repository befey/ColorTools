//
//  TickMarkSettings.hpp
//  SafeguardTools
//
//  Created by T431962 on 7/26/16.
//
//

#ifndef TickMarkSettings_hpp
#define TickMarkSettings_hpp

#include "BleedInfo.h"
#include "AIColor.h"

namespace SafeguardFile
{  
    class TickMarkSettings
    {
    public:
        TickMarkSettings(BleedInfo bleedInfo);
        
        AIRealRect Bounds() const { return bounds; };
        int Offset() const { return offset; };
        float Weight() const { return weight; };
        AIColor Color() const { return color; };
        bool DrawInner() const { return drawInner; };
        bool DrawOuter() const { return drawOuter; };
    private:
        AIRealRect bounds;
        int offset;
        float weight = .5;
        AIColor color;
        bool drawInner = false;
        bool drawOuter = false;
    };
}

#endif /* TickMarkSettings_hpp */

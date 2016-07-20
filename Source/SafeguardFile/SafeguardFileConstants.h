//
//  SafeguardFileConstants.h
//  SafeguardTools
//
//  Created by T431962 on 7/15/16.
//
//

#ifndef SafeguardFileConstants_h
#define SafeguardFileConstants_h

namespace SafeguardFile
{
    enum ProductType {
        INVAL,
        CutSheet,
        Continuous,
        Snapset,
        BusinessStat
    };
    
    static constexpr auto LENGTH_OF_INNER_TICK_PATH = 18;

    constexpr auto KEYLINE_LAYER =              "KEY LINE";
    constexpr auto REG_LAYER =                  "Registration Guide for Proof";
    constexpr auto SLUG_LAYER =                 "SLUG";
    constexpr auto FOREGROUND_LAYER =           "Foreground";
    constexpr auto GUIDE_LAYER =                "Guides";
    constexpr auto BACKGROUND_LAYER =           "Background";
    
    static constexpr auto NO_TOKEN_DESIG =      "F";
}


#endif /* SafeguardFileConstants_h */

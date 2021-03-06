//
//  SafeguardFileConstants.h
//  SafeguardTools
//
//  Created by T431962 on 7/15/16.
//
//

#ifndef SafeguardFileConstants_h
#define SafeguardFileConstants_h

#include <map>

namespace SafeguardFile
{  
    constexpr auto REGISTRATION_COLOR_NAME =    "[Registration]";
    constexpr auto NONE_COLOR_NAME =            "[None]";
    constexpr auto BLACK_COLOR_NAME =           "Black";
    constexpr auto WHITE_COLOR_NAME =           "White";
    constexpr auto CMYK_COLOR_NAME =            "CMYK";
    constexpr auto MICR_BLACK_MAG_COLOR_NAME =  "MICR - BLACK MAG";
    constexpr auto GRIPPER_COLOR_NAME =         "GRIPPER";
    constexpr auto KEYLINE_COLOR_NAME =         "KEYLINE - does not print";
    constexpr auto NUMBERINGRED_COLOR_NAME =    "NUMBERING RED";
    constexpr auto NUMBERINGBLACK_COLOR_NAME =  "NUMBERING BLACK";
    
    enum ProductType
    {
        INVAL,
        CutSheet,
        Continuous,
        Snapset,
        BusinessStat,
        Envelope
    };
    
    static constexpr auto LENGTH_OF_INNER_TICK_PATH = 18;

    constexpr auto KEYLINE_LAYER =              "KEY LINE";
    constexpr auto REG_LAYER =                  "Registration Guide for Proof";
    constexpr auto SLUG_LAYER =                 "SLUG";
    constexpr auto FOREGROUND_LAYER =           "Foreground";
    constexpr auto GUIDE_LAYER =                "Guides";
    constexpr auto BACKGROUND_LAYER =           "Background";
        
    //These values map to the "Value" property of our extension panel select options
    enum class InkMethod
    {
        INVAL = -1,
        NONE,
        Flat,
        Thermo,
        Foil,
        Emboss,
        Magnetic
    };
    
    const std::map<InkMethod, std::string> InkMethodStrings
    {
        {InkMethod::NONE, "NONE"},
        {InkMethod::Flat, "FLAT"},
        {InkMethod::Thermo, "THERMO"},
        {InkMethod::Foil, "FOIL"},
        {InkMethod::Emboss, "EMBOSS"},
        {InkMethod::Magnetic, "MAG"}
    };
    
    //These values map to the "Value" property of our extension panel select options
    enum class TickMarkStyle
    {
        NONE,
        Outer,
        Inner,
        Both,
        BsStyle
    };
    
    const std::map<TickMarkStyle, std::string> TickMarkStyleStrings
    {
        {TickMarkStyle::NONE, "NONE"},
        {TickMarkStyle::Outer, "Outer"},
        {TickMarkStyle::Inner, "Inner"},
        {TickMarkStyle::Both, "Both (Continuous Style)"},
        {TickMarkStyle::BsStyle, "Registration Line (BStat Style)"}
    };
    
    constexpr auto AI_CMYK_BLOCKS =             "sg_CMYK_Blocks";
    constexpr auto AI_CONTINUOUS_REG_TARGET =   "sg_Continuous_Reg";
}

#endif /* SafeguardFileConstants_h */

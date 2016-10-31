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
    constexpr auto PLATE_BLEEDINFO =            "__plate_bleedinfo__";
    constexpr auto PLATE_BLEEDINFO_TIMESTAMP =  "__plate_bleedinfo_timestamp__";
    constexpr auto SG_BLEEDINFO_ARTHANDLES =    "__sg_bleedinfo_arthandles__";
    
    constexpr auto REGISTRATION_COLOR_NAME =    "[Registration]";
    constexpr auto NONE_COLOR_NAME =            "[None]";
    constexpr auto BLACK_COLOR_NAME =           "Black";
    constexpr auto WHITE_COLOR_NAME =           "White";
    constexpr auto CMYK_COLOR_NAME =            "CMYK";
    constexpr auto MICR_BLACK_MAG_COLOR_NAME =  "MICR - BLACK MAG";
    constexpr auto GRIPPER_COLOR_NAME =         "GRIPPER";
    constexpr auto KEYLINE_COLOR_NAME =         "KEYLINE - does not print";
    
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
    
    static constexpr auto NO_TOKEN_DESIG =      "F";
    
    //These values map to the "Value" property of our extension panel select options
    enum class InkMethod
    {
        INVAL = -1,
        NONE,
        Flat,
        Thermo,
        Foil,
        Emboss
    };
    
    const std::map<InkMethod, string> InkMethodStrings
    {
        {InkMethod::NONE, "NONE"},
        {InkMethod::Flat, "FLAT"},
        {InkMethod::Thermo, "THERMO"},
        {InkMethod::Foil, "FOIL"},
        {InkMethod::Emboss, "EMBOSS"}
    };
    
    //These values map to the "Value" property of our extension panel select options
    enum class TickMarkStyle
    {
        NONE,
        Outer,
        Inner,
        Both
    };
}

constexpr auto PATH_TO_AI_RESOURCES =       "../../../Plug-ins.localized/Safeguard/SafeguardTools.aip/Contents/Resources/AiFile/";
constexpr auto AI_CMYK_BLOCKS =             "CMYK_Blocks.ai";
constexpr auto AI_CONTINUOUS_REG_TARGET =   "Continuous_Reg_Target.ai";

#endif /* SafeguardFileConstants_h */

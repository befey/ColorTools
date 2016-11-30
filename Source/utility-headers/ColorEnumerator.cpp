//
//  ColorEnumerator.cpp
//  SafeguardTools
//
//  Created by T431962 on 11/30/16.
//
//

#include "ColorEnumerator.hpp"
#include "ColorFuncs.h"

ColorEnumerator::ColorEnumerator(AIArtHandle art)
:
colorList(GetColorsFromArt(art))
{
}

bool ColorEnumerator::HasRegistrationColor()
{
    for (auto color : colorList)
    {
        if (ColorIsRegistration(color.AiColor()))
        {
            return true;
        }
    }
    return false;
}

vector<AIColor> ColorEnumerator::GetColorsFromArt(AIArtHandle art)
{
    AIBoolean madeReplacement;
    vector<AIColor> colorsInArt;
    
    short type;
    sAIArt->GetArtType(art, &type);
    if ( type == kPlacedArt )
    {
        vector<AIColor> placedColors = GetColorsOfPlacedArt(art);
        colorsInArt.insert(colorsInArt.end(), placedColors.begin(), placedColors.end());
    }
    if ( type == kRasterArt )
    {
        vector<AIColor> placedColors = GetColorsOfRasterArt(art);
        colorsInArt.insert(colorsInArt.end(), placedColors.begin(), placedColors.end());
    }
    
    sAIPathStyle->AdjustObjectAIColors(art, GetColorsCallback, (void*)&colorsInArt, kVisitColorsReadOnly | kVisitGlobalObjectsOnceOnly | kVisitColorsSolidOnly | kVisitColorsIncludeRegistration, &madeReplacement);
    
    return colorsInArt;
}

vector<AIColor> ColorEnumerator::GetColorsOfPlacedArt(AIArtHandle art)
{
    vector<AIColor> results;
    vector<AIColor> groupColors;
    
    sAIFOConversion->EnumerateContents(art, AIFOColorsCallback, (void*)&groupColors);
    
    results.insert(results.end(), groupColors.begin(), groupColors.end());
    
    return results;
}

vector<AIColor> ColorEnumerator::GetColorsOfRasterArt(AIArtHandle art)
{
    vector<AIColor> results;
    AIRasterRecord info;
    
    sAIRaster->GetRasterInfo(art, &info);
    
    if ( info.colorSpace == kGrayColorSpace || info.colorSpace == kAlphaGrayColorSpace )
    {
        results.push_back( GetBlackColor() );
    }
    else if ( info.colorSpace == kRGBColorSpace || info.colorSpace == kAlphaRGBColorSpace ||
             info.colorSpace == kCMYKColorSpace || info.colorSpace == kAlphaCMYKColorSpace ||
             info.colorSpace == kLabColorSpace || info.colorSpace == kAlphaLabColorSpace ||
             info.colorSpace == kIndexedColorSpace || info.colorSpace == kAlphaIndexedColorSpace)
    {
        results.push_back( AIColor{.kind = kFourColor, .c = {.f = {.cyan = 1, .magenta = 1, .yellow = 1, .black = 1} } } );
    }
    return results;
}

void GetColorsCallback(AIColor *color, void *userData, AIErr *result, AIBoolean *altered)
{
    vector<AIColor>* colorsInArt = (vector<AIColor>*)userData;
    colorsInArt->push_back(*color);
}

void AIFOColorsCallback(AIFOContentInfoSelector selector, void *info, void *userData)
{
    vector<AIColor>* colorsInArt = (vector<AIColor>*)userData;
    
    if (selector == kAIFOContentInfoMarksCMYKPlates)
    {
        AIBoolean* cmyk = (AIBoolean*)info;
        if ( cmyk[3] )
        {
            colorsInArt->push_back( GetBlackColor() );
        }
        if ( cmyk[0] || cmyk[1] || cmyk[2] )
        {
            colorsInArt->push_back( AIColor{.kind = kFourColor, .c = {.f = {.cyan = 1, .magenta = 1, .yellow = 1, .black = 1} } } );
        }
    }
    else if (selector == kAIFOContentInfoSpotColor)
    {
        AICustomColorHandle cColorHandle = (AICustomColorHandle)info;
        AIColor color {.kind = kCustomColor, .c = {.c = {.color = cColorHandle, .tint = 0}} };
        colorsInArt->push_back(color);
    }
}

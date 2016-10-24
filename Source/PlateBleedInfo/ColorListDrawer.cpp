//
//  ColorListDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#include "ColorListDrawer.h"
#include "ColorFuncs.h"
#include "BtAteTextFeatures.h"
#include "BtTransformArt.hpp"
#include "SafeguardFileConstants.h"
#include "BtLayer.hpp"

using SafeguardFile::ColorListDrawer;
using SafeguardFile::LaserColorListDrawer;
using SafeguardFile::ContinuousColorListDrawer;
using SafeguardFile::BusStatColorListDrawer;
using SafeguardFile::BleedInfo;

ColorListDrawer::ColorListDrawer(AIRealRect bounds, AIRealPoint anchor, ColorList colorList) :
    BleedTextInfoDrawer(bounds, anchor),
    colorList(colorList)
{
    maxWidth = (bounds.right - bounds.left) * .6;
}

LaserColorListDrawer::LaserColorListDrawer(AIRealRect bounds, ColorList colorList) :
    ColorListDrawer(bounds, {.h = bounds.left + 4, .v = bounds.bottom - 14}, colorList) {};
ContinuousColorListDrawer::ContinuousColorListDrawer(AIRealRect bounds, ColorList colorList) :
    ColorListDrawer(bounds, {.h = bounds.right + 2, .v = bounds.top - 120}, colorList) {};
BusStatColorListDrawer::BusStatColorListDrawer(AIRealRect bounds, ColorList colorList) :
    ColorListDrawer(bounds, {.h = bounds.left, .v = bounds.bottom - 12}, colorList) {};

AIArtHandle LaserColorListDrawer::DoDraw(AIArtHandle resultGroup) const
{
    AIArtHandle colorListArt;
    
    ATE::ITextRange range = SetupTextRange(resultGroup, kHorizontalTextOrientation, &colorListArt);
    
    colorList.GetAsTextRange(range, maxWidth);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(12).Font("Helvetica-Bold").Justification(ATE::kLeftJustify);
    textFeatures.ApplyFeaturesToRange(range);

    return colorListArt;
}

AIArtHandle ContinuousColorListDrawer::DoDraw(AIArtHandle resultGroup) const
{
    AIArtHandle colorListArt;
    
    ATE::ITextRange range = SetupTextRange(resultGroup, kHorizontalTextOrientation, &colorListArt);
    
    RotateArt(colorListArt, anchor, -90);
    
    colorList.GetAsTextRange(range, maxWidth);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(9).Font("Helvetica-Bold").Justification(ATE::kLeftJustify);
    textFeatures.ApplyFeaturesToRange(range);
    
    return colorListArt;
}

AIArtHandle BusStatColorListDrawer::DoDraw(AIArtHandle resultGroup) const
{
    AIArtHandle colorListArt;
    
    ATE::ITextRange range = SetupTextRange(resultGroup, kHorizontalTextOrientation, &colorListArt);
    
    colorList.GetAsTextRange(range, maxWidth);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(7).Font("Helvetica-Condensed-Bold").Justification(ATE::kLeftJustify);
    textFeatures.ApplyFeaturesToRange(range);
    
    return colorListArt;
}

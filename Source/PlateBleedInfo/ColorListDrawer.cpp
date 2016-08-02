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

using SafeguardFile::ColorListDrawer;
using SafeguardFile::LaserColorListDrawer;
using SafeguardFile::ContinuousColorListDrawer;
using SafeguardFile::BusStatColorListDrawer;
using SafeguardFile::BleedInfo;

ColorListDrawer::ColorListDrawer(AIRealRect bounds, AIRealPoint anchor, ColorList colorList) :
    BleedTextInfoDrawer(bounds, anchor),
    colorList(colorList) {};

LaserColorListDrawer::LaserColorListDrawer(AIRealRect bounds, ColorList colorList) :
    ColorListDrawer(bounds, {.h = bounds.left + 4, .v = bounds.bottom - 14}, colorList) {};
ContinuousColorListDrawer::ContinuousColorListDrawer(AIRealRect bounds, ColorList colorList) :
    ColorListDrawer(bounds, {.h = bounds.left + 4, .v = bounds.bottom - 14}, colorList) {};
BusStatColorListDrawer::BusStatColorListDrawer(AIRealRect bounds, ColorList colorList) :
    ColorListDrawer(bounds, {.h = bounds.left + 4, .v = bounds.bottom - 14}, colorList) {};

AIArtHandle LaserColorListDrawer::DoDraw() const
{
    AIArtHandle colorListArt;
    sAITextFrame->NewPointText(kPlaceAboveAll, NULL, kHorizontalTextOrientation, anchor, &colorListArt);
    
    //Create the ATE range
    ATE::TextRangeRef colorListTextRangeRef;
    sAITextFrame->GetATETextRange(colorListArt, &colorListTextRangeRef);
    ATE::ITextRange colorListTextRange(colorListTextRangeRef);
    colorListTextRange.Remove();
    
    colorList.GetAsTextRange(colorListTextRange);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(12.01).Font("Helvetica-Bold").Justification(ATE::kLeftJustify);

    textFeatures.ApplyFeaturesToRange(colorListTextRange);

    return colorListArt;
}
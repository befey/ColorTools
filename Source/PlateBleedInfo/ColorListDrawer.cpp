//
//  ColorListDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#include "ColorListDrawer.h"
#include "ColorFuncs.h"
#include "BleedInfo.h"
#include "BtAteTextFeatures.h"

using SafeguardFile::ColorListDrawer;
using SafeguardFile::LaserColorListDrawer;
using SafeguardFile::ContinuousColorListDrawer;
using SafeguardFile::BusStatColorListDrawer;
using SafeguardFile::BleedInfo;

ColorListDrawer::ColorListDrawer(shared_ptr<BleedInfo> info) : BleedTextInfoDrawer(info) {};
LaserColorListDrawer::LaserColorListDrawer(shared_ptr<BleedInfo> info) : ColorListDrawer(info) {};
ContinuousColorListDrawer::ContinuousColorListDrawer(shared_ptr<BleedInfo> info) : ColorListDrawer(info) {};
BusStatColorListDrawer::BusStatColorListDrawer(shared_ptr<BleedInfo> info) : ColorListDrawer(info) {};

AIArtHandle LaserColorListDrawer::Draw()
{
    AIArtHandle colorListArt;
    AIRealPoint anchor = {.h = p_BleedInfo->rect.left + 4, .v = p_BleedInfo->rect.bottom - 14};
    sAITextFrame->NewPointText(kPlaceAboveAll, NULL, kHorizontalTextOrientation, anchor, &colorListArt);
    
    //Create the ATE range
    ATE::TextRangeRef colorListTextRangeRef;
    sAITextFrame->GetATETextRange(colorListArt, &colorListTextRangeRef);
    ATE::ITextRange colorListTextRange(colorListTextRangeRef);
    colorListTextRange.Remove();
    
    p_BleedInfo->colorList.GetAsTextRange(colorListTextRange);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(12.01).Font("Helvetica-Bold").Justification(ATE::kLeftJustify);

    textFeatures.ApplyFeaturesToRange(colorListTextRange);

    return colorListArt;
}
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
#include "ArtTree.h"

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
    maxHeight = (bounds.top - bounds.bottom) * .6;
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
    
    colorList.GetAsTextRange(range, maxHeight);
    
    RotateArt(colorListArt, anchor, -90);
    MoveArtOutsideBounds(colorListArt, bounds, Direction::Right, 2);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(9).Font("Helvetica-Bold").Justification(ATE::kLeftJustify);
    textFeatures.ApplyFeaturesToRange(range);
    
    DrawContinuousColorBlocks(resultGroup);
    
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

void ContinuousColorListDrawer::DrawContinuousColorBlocks(AIArtHandle resultGroup) const
{
    AIRealRect rect = { .top = bounds.top - 66, .right = bounds.right - 12, .bottom = bounds.top - 66 - 12, .left = bounds.right - 12 - 12 };
    
    AIArtHandle colorBlock = DrawRectangle(rect, resultGroup);
    
    AIPathStyle currPathStyle;
    sAIPathStyle->GetPathStyle(colorBlock, &currPathStyle);
    currPathStyle.strokePaint = false;
    currPathStyle.fillPaint = true;
    currPathStyle.fill = { .color = GetRegistrationColor(), .overprint = true };
    sAIPathStyle->SetPathStyle(colorBlock, &currPathStyle);
    
    vector<BtColor> sortedColorList = colorList.GetColorList();
    
    if (sortedColorList.size() > 0)
    {
        std::for_each(std::begin(sortedColorList)+1, std::end(sortedColorList),
                      [&rect, resultGroup, this](BtColor c)
                      {
                          rect.top -= 36;
                          rect.bottom -= 36;
                          
                          //Make sure the blocks only go about 60% down the page
                          if (! (rect.bottom < bounds.bottom * .6) )
                          {
                              AIArtHandle colorBlock = DrawRectangle(rect, resultGroup);
                              
                              AIPathStyle currPathStyle;
                              sAIPathStyle->GetPathStyle(colorBlock, &currPathStyle);
                              currPathStyle.strokePaint = false;
                              currPathStyle.fillPaint = true;
                              currPathStyle.fill = { .color = c.AiColor(), .overprint = true };
                              sAIPathStyle->SetPathStyle(colorBlock, &currPathStyle);
                          }
                      });
    }
}

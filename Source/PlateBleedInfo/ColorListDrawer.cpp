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
#include "SafeguardJobFileDTO.hpp"
#include "ArtTree.h"
#include "BleedInfo.h"

using SafeguardFile::ColorListDrawer;
using SafeguardFile::LaserColorListDrawer;
using SafeguardFile::ContinuousColorListDrawer;
using SafeguardFile::BusStatColorListDrawer;
using SafeguardFile::BleedInfo;

ColorListDrawer::ColorListDrawer(AIRealRect artboardBounds, AIRealPoint anchor, ColorList colorList)
:
BleedTextInfoDrawer(artboardBounds, anchor),
colorList(colorList)
{
    maxWidth = (artboardBounds.right - artboardBounds.left) * .6;
    maxHeight = (artboardBounds.top - artboardBounds.bottom) * .6;
}

LaserColorListDrawer::LaserColorListDrawer(AIRealRect bounds, ColorList colorList)
:
ColorListDrawer(bounds, {.h = bounds.left + 4, .v = bounds.bottom - 4.5}, colorList)
{};

ContinuousColorListDrawer::ContinuousColorListDrawer(AIRealRect bounds, ColorList colorList)
:
ColorListDrawer(bounds, {.h = bounds.right, .v = bounds.top + ((bounds.bottom - bounds.top) * .4)}, colorList)
{};

BusStatColorListDrawer::BusStatColorListDrawer(AIRealRect bounds, ColorList colorList)
:
ColorListDrawer(bounds, {.h = bounds.left, .v = bounds.bottom - 4.5}, colorList)
{};

AIArtHandle ColorListDrawer::Draw(AIArtHandle resultGroup) const
{
    AIArtHandle colorListArt = DrawerSpecificSteps(resultGroup);
    colorList.WriteColorListToArtDictionary(colorListArt);

    return colorListArt;
}

AIArtHandle LaserColorListDrawer::DrawerSpecificSteps(AIArtHandle resultGroup) const
{
    AIArtHandle colorListArt;
    
    ATE::ITextRange range = SetupTextRange(resultGroup, maxWidth, ATE::kLeftJustify, kHorizontalTextOrientation, &colorListArt);
    
    colorList.AsTextRange(range);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(12).Font("Helvetica-Bold").Justification(ATE::kLeftJustify);
    textFeatures.ApplyFeaturesToRange(range);

    FitTextFrameToContents(colorListArt);
    
    return colorListArt;
}

AIArtHandle ContinuousColorListDrawer::DrawerSpecificSteps(AIArtHandle resultGroup) const
{
    AIArtHandle colorListGroup = NULL;
    sAIArt->NewArt(kGroupArt, kPlaceInsideOnTop, resultGroup, &colorListGroup);
    
    AIArtHandle colorListArt = NULL;
    
    //Passing maxHeight here because we're going to rotate
    ATE::ITextRange range = SetupTextRange(colorListGroup, maxHeight, ATE::kLeftJustify, kHorizontalTextOrientation, &colorListArt);
    
    colorList.AsTextRange(range);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(9).Font("Helvetica-Bold").Justification(ATE::kLeftJustify);
    textFeatures.ApplyFeaturesToRange(range);
    
    FitTextFrameToContents(colorListArt);
    
    RotateArt(colorListArt, anchor, -90);
    MoveArtOutsideBounds(colorListArt, artboardBounds, Direction::Right, 0);
    
    DrawContinuousColorBlocks(colorListGroup);
    
    return colorListGroup;
}

AIArtHandle BusStatColorListDrawer::DrawerSpecificSteps(AIArtHandle resultGroup) const
{
    AIArtHandle colorListArt;
    
    ATE::ITextRange range = SetupTextRange(resultGroup, maxWidth, ATE::kLeftJustify, kHorizontalTextOrientation, &colorListArt);
    
    colorList.AsTextRange(range);
    
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(7).Font("Helvetica-Condensed-Bold").Justification(ATE::kLeftJustify);
    textFeatures.ApplyFeaturesToRange(range);
    
    FitTextFrameToContents(colorListArt);
    
    return colorListArt;
}

void ContinuousColorListDrawer::DrawContinuousColorBlocks(AIArtHandle resultGroup) const
{
    AIRealRect rect = { .top = artboardBounds.top - 66, .right = artboardBounds.right - 12, .bottom = artboardBounds.top - 66 - 12, .left = artboardBounds.right - 12 - 12 };
    
    AIArtHandle colorBlock = DrawRectangle(rect, resultGroup);
    
    AIPathStyle currPathStyle;
    sAIPathStyle->GetPathStyle(colorBlock, &currPathStyle);
    currPathStyle.strokePaint = false;
    currPathStyle.fillPaint = true;
    currPathStyle.fill = { .color = BtColor::RegistrationColor(), .overprint = true };
    sAIPathStyle->SetPathStyle(colorBlock, &currPathStyle);
       
    if (colorList.size() > 0 && colorList[0].Kind() == kFourColor)
    {
        vector<AIColor> cmykColors =
        {
            {.kind = kFourColor, .c.f.cyan = 1, .c.f.magenta = 0, .c.f.yellow = 0, .c.f.black = 0},
            {.kind = kFourColor, .c.f.cyan = 0, .c.f.magenta = 1, .c.f.yellow = 0, .c.f.black = 0},
            {.kind = kFourColor, .c.f.cyan = 0, .c.f.magenta = 0, .c.f.yellow = 1, .c.f.black = 0},
            {BtColor::BlackColor().AiColor()}
        };
        for (auto c : cmykColors)
        {
            rect.top -= 36;
            rect.bottom -= 36;
            
            //Make sure the blocks only go about 60% down the page
            if (! (rect.bottom < artboardBounds.bottom * .6) )
            {
                AIArtHandle colorBlock = DrawRectangle(rect, resultGroup);
                
                AIPathStyle currPathStyle;
                sAIPathStyle->GetPathStyle(colorBlock, &currPathStyle);
                currPathStyle.strokePaint = false;
                currPathStyle.fillPaint = true;
                currPathStyle.fill = { .color = c, .overprint = true };
                sAIPathStyle->SetPathStyle(colorBlock, &currPathStyle);
            }
        }
    }
    
    if (colorList.size() > 0)
    {
        std::for_each(colorList.begin()+1, colorList.end(),
                      [&rect, resultGroup, this](BtColor c)
                      {
                          rect.top -= 36;
                          rect.bottom -= 36;
                          
                          //Make sure the blocks only go about 60% down the page
                          if (! (rect.bottom < artboardBounds.bottom * .6) )
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

bool ShouldCreateColorListDrawable::Get() const
{
    if (!settings.shouldDrawBleedInfo) //We'll have a NoneDrawer here, so we need to "draw" the None color list
    {
        return true;
    }
    
    if (!pluginArt)
    {
        return true;
    }
    
    DictionaryWriter dw(pluginArt);
    string json;
    dw.GetStringDataFromIdentifier(json, PlateBleedInfo::PLATE_BLEEDINFO);
    PlateBleedInfo::PlateDTO dto(json);
    
    if (settings.artboardBounds != dto.bounds)
    {
        return true;
    }
    
    AIArtHandle resultGroup = NULL;
    sAIPluginGroup->GetPluginArtResultArt(pluginArt, &resultGroup);
    
    AIArtHandle existingColorListArtHandle = NULL;
    existingColorListArtHandle = DictionaryWriter(resultGroup).GetArtHandleFromIdentifier(drawable->GetDrawer()->GetDictionaryLabel(resultGroup));
    if (existingColorListArtHandle)
    {
        ColorList existingColorList(vector<AIColor>{});
        existingColorList.ReadColorListFromArtDictionary(existingColorListArtHandle);
        if ( existingColorList == settings.colorList )
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return true;
}

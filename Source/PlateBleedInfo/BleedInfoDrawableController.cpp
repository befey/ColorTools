//
//  BleedInfoDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#include "BleedInfoDrawableController.h"
#include "SafeguardToolsPlugin.h"
#include "ArtTree.h"
#include "BtLayer.hpp"
#include "GetIllustratorErrorCode.h"


using PlateBleedInfo::BleedInfoDrawableController;

BleedInfoDrawableController::BleedInfoDrawableController(const BleedInfo& bleedInfo)
:
bleedInfo(bleedInfo)
{
    drawables.push_back(DrawableFactory().GetDrawable( bleedInfo.TickMarkSettings(), bleedInfo.BleedInfoPluginArtHandle()) );
    
    SafeguardFile::ProductType pt = bleedInfo.PlateNumber().GetProductType();
    
    drawables.push_back(DrawableFactory().GetDrawable( ColorListDrawerSettings(pt, bleedInfo.ArtboardBounds(), bleedInfo.ColorList()), bleedInfo.BleedInfoPluginArtHandle()) );
    drawables.push_back(DrawableFactory().GetDrawable( FileNameDateDrawerSettings(pt, bleedInfo.ArtboardBounds(), bleedInfo.PlateNumber(), bleedInfo.Token(), bleedInfo.LastModified()), bleedInfo.BleedInfoPluginArtHandle()) );
    
    if (bleedInfo.ShouldAddCmykBlocks())
    {
        AIRealRect abBounds = bleedInfo.ArtboardBounds();
        AIRealRect bounds = { //CMYK Blocks are 325x25px
            .left = abBounds.left + ((abBounds.right - abBounds.left)/2) - (325/2),
            .top = abBounds.top + 5 + 25,
            .right = abBounds.right - ((abBounds.right - abBounds.left)/2) + (325/2),
            .bottom = abBounds.top + 5
        };
        
        drawables.push_back(DrawableFactory().GetDrawable( SgSymbolDrawerSettings(bleedInfo.ArtboardBounds(), bounds, SafeguardFile::AI_CMYK_BLOCKS), bleedInfo.BleedInfoPluginArtHandle()) );
    }
    
    if (bleedInfo.PlateNumber().GetProductType() == SafeguardFile::Continuous)
    {
        AIRealRect abBounds = bleedInfo.ArtboardBounds();
        AIRealRect bounds = { //Reg block is 24.3x36px
            .left = abBounds.left,
            .top = abBounds.top - 42,
            .right = abBounds.left + 24.3,
            .bottom = abBounds.top - 42 - 36
        };
        
        drawables.push_back(DrawableFactory().GetDrawable( SgSymbolDrawerSettings(bleedInfo.ArtboardBounds(), bounds, SafeguardFile::AI_CONTINUOUS_REG_TARGET), bleedInfo.BleedInfoPluginArtHandle()) );
    }
}

AIArtHandle BleedInfoDrawableController::Draw() const
{
    if (bleedInfo.ShouldDrawBleedInfo())
    {
        if (bleedInfo.BleedInfoPluginArtHandle())
        {
             return Update(bleedInfo.BleedInfoPluginArtHandle());
        }
        else
        {
            return Add();
        }
    }
    else
    {
        return Remove();
    }
}

AIArtHandle BleedInfoDrawableController::Add() const
{
    AIArtHandle pluginGroupArt = NULL;
    
    BtLayer foregroundLayer(SafeguardFile::FOREGROUND_LAYER);
    AIArtHandle prep = foregroundLayer.GetLayerGroupArt();
    
    if (prep == NULL)
    {
        prep = GetGroupArtOfFirstEditableLayer();
    }
    
    sAIArt->NewArt(kPluginArt, kPlaceInsideOnBottom, prep, &pluginGroupArt);
    AIErr err = sAIPluginGroup->UseAIPluginGroup(pluginGroupArt, gPlugin->GetBleedInfoPluginGroupHandle());
    string error = GetIllustratorErrorCode(err);
    
    CreateResultArt(pluginGroupArt);
    
    return pluginGroupArt;
}

AIArtHandle BleedInfoDrawableController::Update(AIArtHandle pluginGroupArt) const
{
    CreateResultArt(pluginGroupArt);
    
    return pluginGroupArt;
}

AIArtHandle BleedInfoDrawableController::Remove() const
{
    if (bleedInfo.BleedInfoPluginArtHandle())
    {
        sAIArt->DisposeArt(bleedInfo.BleedInfoPluginArtHandle());
    }
    return NULL;
}

AIArtHandle BleedInfoDrawableController::CreateResultArt(AIArtHandle pluginGroupArt) const
{
    AIArtHandle resultGroup;
    sAIPluginGroup->GetPluginArtResultArt(pluginGroupArt, &resultGroup);
        
    for (auto drawable : drawables)
    {
        if (drawable)
        {
            drawable->Clear(resultGroup);
            drawable->Draw(resultGroup);
        }
    }
    
    return pluginGroupArt;
}

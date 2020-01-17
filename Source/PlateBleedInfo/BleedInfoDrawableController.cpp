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
#include "BtArtHandle.hpp"
#include "GetIllustratorErrorCode.h"
#include "DrawDrawableCommand.hpp"


using PlateBleedInfo::BleedInfoDrawableController;

BleedInfoDrawableController::BleedInfoDrawableController(const BleedInfo& bleedInfo)
:
bleedInfo(bleedInfo)
{
    //Order here matters! Drawables will be placed in order, and TODO: will not be allowed to overlap previous
    drawables.push_back(DrawableFactory().GetDrawable( bleedInfo.TickMarkSettings(), bleedInfo.BleedInfoPluginArtHandle()) );
    
    drawables.push_back(DrawableFactory().GetDrawable( FileNameDateDrawerSettings(bleedInfo), bleedInfo.BleedInfoPluginArtHandle()) );
    
    drawables.push_back(DrawableFactory().GetDrawable( ColorListDrawerSettings(bleedInfo), bleedInfo.BleedInfoPluginArtHandle()) );
    
    drawables.push_back(DrawableFactory().GetDrawable( SgSymbolDrawerSettings(bleedInfo, SafeguardFile::AI_CMYK_BLOCKS, bleedInfo.ShouldAddCmykBlocks()), bleedInfo.BleedInfoPluginArtHandle()) );
    
    bool shouldDrawContReg = bleedInfo.PlateNumber().GetProductType() == SafeguardFile::Continuous;
    drawables.push_back(DrawableFactory().GetDrawable( SgSymbolDrawerSettings(bleedInfo, SafeguardFile::AI_CONTINUOUS_REG_TARGET, shouldDrawContReg), bleedInfo.BleedInfoPluginArtHandle()) );
}

AIArtHandle BleedInfoDrawableController::Draw() const
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

AIArtHandle BleedInfoDrawableController::Add() const
{
    AIArtHandle pluginGroupArt = nullptr;
    
    BtLayer foregroundLayer(SafeguardFile::FOREGROUND_LAYER, DoNotCreateLayer);
    AIArtHandle prep = foregroundLayer.GetLayerGroupArt();
    
    if (prep == nullptr)
    {
        prep = GetGroupArtOfFirstEditableLayer();
    }
    
    if (prep == nullptr)
    {
        sAIUser->MessageAlert(ai::UnicodeString("No visible, editable layer found."));
        return nullptr;
    }
    
    BtLayer pluginArtLayer(BtArtHandle(prep).Layer());
    pluginArtLayer.MakeEditable();
    
    sAIArt->NewArt(kPluginArt, kPlaceInsideOnTop, prep, &pluginGroupArt);
    AIErr err = sAIPluginGroup->UseAIPluginGroup(pluginGroupArt, gPlugin->GetBleedInfoPluginGroupHandle());
    string error = GetIllustratorErrorCode(err);
    
    pluginArtLayer.ResetEditable();
    
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
    return nullptr;
}

AIArtHandle BleedInfoDrawableController::CreateResultArt(AIArtHandle pluginGroupArt) const
{
    AIArtHandle resultGroup = nullptr;
    sAIPluginGroup->GetPluginArtResultArt(pluginGroupArt, &resultGroup);
    
    BtLayer pluginGroupLayer(BtArtHandle(resultGroup).Layer());
    pluginGroupLayer.MakeEditable();
    
    for (auto drawable : drawables)
    {
        DrawDrawableCommand(drawable, resultGroup).Execute();
    }
    
    pluginGroupLayer.PutArtAtTopOfLayer(pluginGroupArt);
    
    pluginGroupLayer.ResetEditable();
    
    return pluginGroupArt;
}

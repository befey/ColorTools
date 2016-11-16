//
//  BleedInfoDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#include "BleedInfoDrawer.h"
#include "SafeguardToolsPlugin.h"
#include "ArtTree.h"
#include "BtLayer.hpp"
#include "GetIllustratorErrorCode.h"


using SafeguardFile::BleedInfoDrawer;

BleedInfoDrawer::BleedInfoDrawer(BleedInfo bleedInfo)
:
bleedInfo(bleedInfo)
{
    AddDrawer( MakeTickMarkDrawer(bleedInfo.TickMarkSettings()) );
    
    ProductType pt = bleedInfo.PlateNumber().GetProductType();
    
    AddDrawer( MakeColorListDrawer(pt, bleedInfo.ArtboardBounds(), bleedInfo.ColorList()) );
    AddDrawer( MakeFileNameDateDrawer(pt, bleedInfo.ArtboardBounds(), bleedInfo.PlateNumber(), bleedInfo.Token(), bleedInfo.LastModified()) );
    
    if (bleedInfo.ShouldAddCmykBlocks())
    {
        AIRealRect abBounds = bleedInfo.ArtboardBounds();
        AIRealRect bounds = { //CMYK Blocks are 325x25px
            .left = abBounds.left + ((abBounds.right - abBounds.left)/2) - (325/2),
            .top = abBounds.top + 5 + 25,
            .right = abBounds.right - ((abBounds.right - abBounds.left)/2) + (325/2),
            .bottom = abBounds.top + 5
        };
        
        AddDrawer( MakeSgSymbolDrawer(bounds, AI_CMYK_BLOCKS) );
    }
    
    if (bleedInfo.PlateNumber().GetProductType() == Continuous)
    {
        AIRealRect abBounds = bleedInfo.ArtboardBounds();
        AIRealRect bounds = { //Reg block is 24.3x36px
            .left = abBounds.left,
            .top = abBounds.top - 42,
            .right = abBounds.left + 24.3,
            .bottom = abBounds.top - 42 - 36
        };
        
        AddDrawer( MakeSgSymbolDrawer(bounds, AI_CONTINUOUS_REG_TARGET) );
    }
}

BleedInfoDrawer& BleedInfoDrawer::AddDrawer(shared_ptr<IDrawer> val)
{
    if (val != nullptr)
    {
        drawers.push_back(val);
    }
    return *this;
}

shared_ptr<IDrawer> BleedInfoDrawer::MakeColorListDrawer(ProductType pt, AIRealRect artboardBounds, ColorList colorList)
{
    if (pt == ProductType::BusinessStat)
    {
        return make_shared<BusStatColorListDrawer>(artboardBounds, colorList);
    }
    else if (pt == ProductType::Continuous)
    {
        return make_shared<ContinuousColorListDrawer>(artboardBounds, colorList);
    }
    else if (pt == ProductType::CutSheet || pt == ProductType::Envelope)
    {
        return make_shared<LaserColorListDrawer>(artboardBounds, colorList);
    }
    
    return nullptr;
}

shared_ptr<IDrawer> BleedInfoDrawer::MakeFileNameDateDrawer(ProductType pt, AIRealRect artboardBounds, PlateNumber plateNumber, string token, tm lastModified)
{
    if (pt == ProductType::BusinessStat)
    {
        return make_shared<BusStatFileNameDateDrawer>(artboardBounds, plateNumber, token, lastModified);
    }
    else if (pt == ProductType::Continuous)
    {
        return make_shared<ContinuousFileNameDateDrawer>(artboardBounds, plateNumber, token, lastModified);
    }
    else if (pt == ProductType::CutSheet || pt == ProductType::Envelope)
    {
        return make_shared<LaserFileNameDateDrawer>(artboardBounds, plateNumber, token, lastModified);
    }
    
    return nullptr;
}

shared_ptr<IDrawer> BleedInfoDrawer::MakeTickMarkDrawer(TickMarkSettings tmSettings)
{
    return make_shared<TickMarkDrawer>(tmSettings);
}

shared_ptr<IDrawer> BleedInfoDrawer::MakeSgSymbolDrawer(AIRealRect artboardBounds, string symbolName)
{
    return make_shared<SgSymbolDrawer>(artboardBounds, symbolName);
}

AIArtHandle BleedInfoDrawer::DoDraw(AIArtHandle pluginGroupArt) const
{
    if (pluginGroupArt)
    {
        return Update(pluginGroupArt);
    }
    else
    {
        return Add();
    }
    return NULL;
}

AIArtHandle BleedInfoDrawer::Add() const
{
    AIArtHandle pluginGroupArt;
    
    BtLayer foregroundLayer(FOREGROUND_LAYER);
    AIArtHandle prep = foregroundLayer.GetLayerGroupArt();
    
    AIErr err = sAIArt->NewArt(kPluginArt, kPlaceInsideOnBottom, prep, &pluginGroupArt);
    string error = GetIllustratorErrorCode(err);
    sAIPluginGroup->UseAIPluginGroup(pluginGroupArt, gPlugin->GetBleedInfoPluginGroupHandle());
    
    CreateResultArt(pluginGroupArt);
    
    return pluginGroupArt;
}

AIArtHandle BleedInfoDrawer::Update(AIArtHandle pluginGroupArt) const
{
    CreateResultArt(pluginGroupArt);
    
    return pluginGroupArt;
}

AIArtHandle BleedInfoDrawer::CreateResultArt(AIArtHandle pluginGroupArt) const
{
    AIArtHandle resultGroup;
    sAIPluginGroup->GetPluginArtResultArt(pluginGroupArt, &resultGroup);
    
    ClearResultArt(resultGroup);
    
    for (auto drawer : drawers)
    {
        drawer->Draw(resultGroup);
    }
    
    return pluginGroupArt;
}

void BleedInfoDrawer::ClearResultArt(AIArtHandle resultGroupArt) const
{
    // Destroy all art that exists below the result art.
    AIArtHandle child = NULL;
    sAIArt->GetArtFirstChild(resultGroupArt, &child);
    while (child != NULL)
    {
        sAIArt->DisposeArt(child);
        sAIArt->GetArtFirstChild(resultGroupArt, &child);
    }
}

void BleedInfoDrawer::Remove(AIArtHandle& pluginGroupArt) const
{
    sAIArt->DisposeArt(pluginGroupArt);
    pluginGroupArt = NULL;
}

//
//  BleedInfoDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#include "BleedInfoDrawer.h"
#include "SafeguardToolsPlugin.h"
#include "DictionaryWriter.h"
#include "ArtTree.h"
#include "BtLayer.hpp"
#include "GetIllustratorErrorCode.h"


using SafeguardFile::BleedInfoDrawer;

AIArtHandle BleedInfoDrawer::DoDraw() const
{
    unique_ptr<DictionaryWriter> dw = make_unique<DictionaryWriter>();
    if (dw->CheckDictionaryForArtObjectWithIdentifier(PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex))
    {
        AIArtHandle foundArt = dw->GetArtHandleFromIdentifier(PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex);
        if (foundArt)
        {
            Update(foundArt);
        }
        else
        {
            dw->RemoveIdentifierFromDictionary(PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex);
        }
    }
    else
    {
        Add();
    }
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
    
    unique_ptr<DictionaryWriter> dw = make_unique<DictionaryWriter>();
    dw->AddArtHandleToDictionary(pluginGroupArt, PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex);
    
    return pluginGroupArt;
}

AIArtHandle BleedInfoDrawer::Update(AIArtHandle pluginGroupArt) const
{
    CreateResultArt(pluginGroupArt);
    sAIPluginGroup->MarkPluginArtDirty(pluginGroupArt);
    return pluginGroupArt;
}

AIArtHandle BleedInfoDrawer::CreateResultArt(AIArtHandle pluginGroupArt) const
{
    vector<AIArtHandle> resultHandles;
    
    for (auto drawer : drawers)
    {
        resultHandles.push_back(drawer->Draw());
    }
    
    AIArtHandle resultGroupArt;
    ASErr err = sAIArt->NewArt(kGroupArt, kPlaceInsideOnBottom, GetGroupArtOfFirstEditableLayer(), &resultGroupArt);
    string error = GetIllustratorErrorCode(err);
    for (auto art : resultHandles)
    {
        if (art != NULL)
        {
            PutArtInGroup(art, resultGroupArt);
        }
    }
    
    sAIPluginGroup->SetPluginArtResultArt(pluginGroupArt, resultGroupArt);
    
    return pluginGroupArt;
}


void BleedInfoDrawer::Remove(AIArtHandle pluginGroupArt) const
{
    unique_ptr<DictionaryWriter> dw = make_unique<DictionaryWriter>();
    dw->RemoveIdentifierFromDictionary(PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex);
    sAIArt->DisposeArt(pluginGroupArt);
}
/*
 ASErr BleedInfo::PluginGroupNotify(AIPluginGroupMessage* message)
 {
 return kUnhandledMsgErr;
 }
 
 ASErr BleedInfo::PluginGroupUpdate(AIPluginGroupMessage* message)
 {
 return kUnhandledMsgErr;
 }
 */
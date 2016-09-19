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

AIArtHandle BleedInfoDrawer::DoDraw(AIArtHandle resultGroup) const
{
    DictionaryWriter dw;
    if (dw.CheckDictionaryForArtObjectWithIdentifier(PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex))
    {
        AIArtHandle foundArt = dw.GetArtHandleFromIdentifier(PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex);
        if (foundArt)
        {
            return Update(foundArt);
        }
        else
        {
            dw.RemoveIdentifierFromDictionary(PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex);
        }
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
    sAIPluginGroup->MarkPluginArtSilent(pluginGroupArt);
    
    DictionaryWriter dw;
    dw.AddArtHandleToDictionary(pluginGroupArt, PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex);
    
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
    AIArtHandle resultGroup;
    sAIPluginGroup->GetPluginArtResultArt(pluginGroupArt, &resultGroup);
    
    ClearResultArt(resultGroup);
    
    for (auto drawer : drawers)
    {
        resultHandles.push_back(drawer->Draw(resultGroup));
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

void BleedInfoDrawer::Remove(AIArtHandle pluginGroupArt) const
{
    DictionaryWriter dw;
    dw.RemoveIdentifierFromDictionary(PLATE_BLEED_INFO_GROUP_LABEL, artboardIndex);
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

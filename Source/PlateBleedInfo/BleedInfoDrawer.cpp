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


using SafeguardFile::BleedInfoDrawer;

void BleedInfoDrawer::Draw() const
{
    unique_ptr<DictionaryWriter> dw = make_unique<DictionaryWriter>();
    if (dw->CheckDictionaryForArtObjectWithIdentifier(PLATE_BLEED_INFO_GROUP_LABEL, p_BleedInfo->artboardIndex))
    {
        AIArtHandle foundArt = dw->GetArtHandleFromIdentifier(PLATE_BLEED_INFO_GROUP_LABEL, p_BleedInfo->artboardIndex);
        if (foundArt)
        {
            Update(foundArt);
        }
        else
        {
            dw->RemoveIdentifierFromDictionary(PLATE_BLEED_INFO_GROUP_LABEL, p_BleedInfo->artboardIndex);
        }
    }
    else
    {
        Add();
    }
}

void BleedInfoDrawer::Add() const
{
    AIArtHandle pluginGroupArt;
    sAIArt->NewArt(kPluginArt, kPlaceAboveAll, NULL, &pluginGroupArt);
    
    sAIPluginGroup->UseAIPluginGroup(pluginGroupArt, gPlugin->GetBleedInfoPluginGroupHandle());
    BtLayer foregroundLayer(FOREGROUND_LAYER);
    foregroundLayer.PutArtAtTopOfLayer(pluginGroupArt);

    CreateResultArt(pluginGroupArt);
    
    unique_ptr<DictionaryWriter> dw = make_unique<DictionaryWriter>();
    dw->AddArtHandleToDictionary(pluginGroupArt, PLATE_BLEED_INFO_GROUP_LABEL, p_BleedInfo->artboardIndex);
}

void BleedInfoDrawer::Update(AIArtHandle pluginGroupArt) const
{
    AIArtHandle resultArt;
    sAIPluginGroup->GetPluginArtResultArt(pluginGroupArt, &resultArt);
    sAIArt->DisposeArt(resultArt);
    CreateResultArt(pluginGroupArt);
}

void BleedInfoDrawer::CreateResultArt(AIArtHandle pluginGroupArt) const
{
    vector<AIArtHandle> resultHandles;
    
    resultHandles.push_back(fileNameDateDrawer->Draw());
    resultHandles.push_back(colorListDrawer->Draw());
    resultHandles.push_back(tickMarkDrawer->Draw());
    
    AIArtHandle resultGroupArt;
    sAIArt->NewArt(kGroupArt, kPlaceAboveAll, NULL, &resultGroupArt);
    
    for (auto art : resultHandles)
    {
        if (art != NULL)
        {
            PutArtInGroup(art, resultGroupArt);
        }
    }
    
    sAIPluginGroup->SetPluginArtResultArt(pluginGroupArt, resultGroupArt);
}


void BleedInfoDrawer::Remove(AIArtHandle pluginGroupArt) const
{
    unique_ptr<DictionaryWriter> dw = make_unique<DictionaryWriter>();
    dw->RemoveIdentifierFromDictionary(PLATE_BLEED_INFO_GROUP_LABEL, p_BleedInfo->artboardIndex);
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
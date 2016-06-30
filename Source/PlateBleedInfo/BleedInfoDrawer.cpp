//
//  BleedInfoDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#include "BleedInfoDrawer.h"
#include "SafeguardToolsPlugin.h"
#include "DictFuncs.h"
#include "ArtTree.h"


using SafeguardFile::BleedInfoDrawer;

void BleedInfoDrawer::Draw() const
{
    if (CheckDictionaryForArtObjectWithIdentifier(ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), p_BleedInfo->artboardIndex))
    {
        AIArtHandle foundArt = GetArtHandleFromIdentifier(ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), p_BleedInfo->artboardIndex);
        if (foundArt)
        {
            Update(foundArt);
        }
        else
        {
            RemoveIdentifierFromDictionary(ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), p_BleedInfo->artboardIndex);
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
    PutArtAtTopOfLayer(pluginGroupArt, FOREGROUND_LAYER);

    CreateResultArt(pluginGroupArt);
    
    AddArtUIDToDictionary(pluginGroupArt, ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), p_BleedInfo->artboardIndex);
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
    
    resultHandles.push_back(p_BleedInfo->fileNameDateDrawer->Draw());
    resultHandles.push_back(p_BleedInfo->colorListDrawer->Draw());
    resultHandles.push_back(p_BleedInfo->tickMarkDrawer->Draw());
    
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
    RemoveIdentifierFromDictionary(ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), p_BleedInfo->artboardIndex);
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
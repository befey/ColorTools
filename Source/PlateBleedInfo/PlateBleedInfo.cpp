//
//  PlateBleedInfo.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/7/16.
//
//

#include "PlateBleedInfo.h"
#include "SafeguardToolsPlugin.h"
#include "AIArtboard.h"
#include "ATEFuncs.h"
#include "DictFuncs.h"
#include "ArtTree.h"


void PlateBleedInfo::AddPlateBleedInfo()
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardID count;
    sAIArtboard->GetCount(abList, count);
    ai::ArtboardProperties props;
    
    RemovePlateBleedInfo();
    
    for (int i = 0; i < count; i++)
    {
        sAIArtboard->GetArtboardProperties(abList, i, props);
        AIRealRect rect;
        props.GetPosition(rect);
        
        AIArtHandle pluginGroupArt;
        sAIArt->NewArt(kPluginArt, kPlaceAboveAll, NULL, &pluginGroupArt);
        sAIPluginGroup->UseAIPluginGroup(pluginGroupArt, gPlugin->GetCreatePlateBleedInfoPluginGroupHandle());
        
        PutArtAtTopOfLayer(pluginGroupArt, FOREGROUND_LAYER);
        
        AIArtHandle resultArt, plateInfoText, colorListText;
        sAIArt->NewArt(kGroupArt, kPlaceAboveAll, NULL, &resultArt);
        
        AIRealPoint anchor = {.h = rect.right - 4, .v = rect.bottom - 14};
        sAITextFrame->NewPointText(kPlaceInsideOnTop, resultArt, kHorizontalTextOrientation, anchor, &plateInfoText);
        
        //Create the ATE range
        ATE::TextRangeRef plateInfoTextRangeRef;
        sAITextFrame->GetATETextRange(plateInfoText, &plateInfoTextRangeRef);
        ATE::ITextRange plateInfoTextRange(plateInfoTextRangeRef);
        plateInfoTextRange.Remove();
        
        gPlugin->sgJobFile->plates[i].GetPlateNumberAndDateAsTextRange(plateInfoTextRange);
                
        anchor = {.h = rect.left + 4, .v = rect.bottom - 14};
        sAITextFrame->NewPointText(kPlaceInsideOnTop, resultArt, kHorizontalTextOrientation, anchor, &colorListText);
        
        //Create the ATE range
        ATE::TextRangeRef colorListTextRangeRef;
        sAITextFrame->GetATETextRange(colorListText, &colorListTextRangeRef);
        ATE::ITextRange colorListTextRange(colorListTextRangeRef);
        colorListTextRange.Remove();
        
        gPlugin->sgJobFile->plates[i].GetColorListAsTextRange(colorListTextRange);
       
        sAIPluginGroup->SetPluginArtResultArt(pluginGroupArt, resultArt);
        
        AddArtUIDToDictionary(pluginGroupArt, ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), i);
    }
}

void PlateBleedInfo::RemovePlateBleedInfo()
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardID count;
    sAIArtboard->GetCount(abList, count);
    
    for (int i = 0; i < count; i++)
    {
        if (CheckDictionaryForArtObjectWithIdentifier(ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), i))
        {
            AIArtHandle foundArt = GetArtHandleFromIdentifier(ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), i);
            if (foundArt)
            {
                RemoveIdentifierFromDictionary(ai::UnicodeString(PLATE_BLEED_INFO_GROUP_LABEL), i);
                sAIArt->DisposeArt(foundArt);
            }
        }
    }
}

void PlateBleedInfo::UpdatePlateBleedInfo(AINotifierMessage* message)
{
    
//    ai::ArtboardRef ref = (ai::ArtboardRef)(message->notifyData);
//    ai::ArtboardProperties props = ai::ArtboardProperties(ref);
//    ai::UnicodeString name;
//    props.GetName(name);
//    string n = name.getInStdString(kAIPlatformCharacterEncoding);
//    int foo = 0;
    if (gPlugin->sgJobFile)
    {
        AddPlateBleedInfo();
    }
    
}

ASErr PlateBleedInfo::PluginGroupNotify(AIPluginGroupMessage* message)
{
    return kUnhandledMsgErr;
}

ASErr PlateBleedInfo::PluginGroupUpdate(AIPluginGroupMessage* message)
{
    return kUnhandledMsgErr;
}
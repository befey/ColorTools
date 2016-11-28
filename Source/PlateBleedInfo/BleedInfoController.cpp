//
//  BleedInfoController.cpp
//  SafeguardTools
//
//  Created by T431962 on 11/28/16.
//
//

#include "BleedInfoController.hpp"
#include "DictionaryWriter.h"
#include "BleedInfoPluginArtToArtboardMatcher.hpp"
#include "SafeguardJobFile.h"

using PlateBleedInfo::BleedInfoController;

void BleedInfoController::HandleCropAreaNotification()
{
    if ( ShouldDoUpdate() )
    {
        SafeguardFile::SafeguardJobFile().UpdateBleedInfo();
        DictionaryWriter dw;
        dw.AddAIRealToDictionary(sAIArt->GetGlobalTimeStamp(), SafeguardFile::PLATE_BLEEDINFO_TIMESTAMP);
    }
}

void BleedInfoController::HandleCreateMenu()
{
    SafeguardFile::SafeguardJobFile().UpdateBleedInfo();
}

void BleedInfoController::HandleEditMenu()
{
    SafeguardFile::SafeguardJobFile().EditBleedInfo();
}

bool BleedInfoController::ShouldDoUpdate()
{
    if (PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        size_t gTimeStamp = sAIArt->GetGlobalTimeStamp();
        DictionaryWriter dw;
        AIReal aTSDict = dw.GetAIRealFromIdentifier(SafeguardFile::PLATE_BLEEDINFO_TIMESTAMP);
        
        if ( gTimeStamp != aTSDict )
        {
            return true;
        }
    }
    return false;
}

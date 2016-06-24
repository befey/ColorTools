//
//  SafeguardJobFile.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "SafeguardJobFile.h"
#include "DictFuncs.h"

using SafeguardFile::SafeguardJobFile;

void SafeguardJobFile::Update()
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardID count;
    sAIArtboard->GetCount(abList, count);
    
    for (int i = 0; i < count; i++)
    {
        plates.push_back(Plate(i));
    }
    
    if (ShouldDrawBleedInfo()) {
        AddBleedInfo();
    }
}


void SafeguardJobFile::AddBleedInfo()
{
    if (ShouldDrawBleedInfo())
    {
        for (auto plate : plates)
        {
            plate.AddBleedInfo();
        }
    }
}

void SafeguardJobFile::RemoveBleedInfo()
{
    for (auto plate : plates)
    {
        plate.RemoveBleedInfo();
    }
}

bool SafeguardJobFile::ShouldDrawBleedInfo()
{
    //if ( CheckDictionaryForArtObjectWithIdentifier(ai::UnicodeString(SafeguardFile::PLATE_BLEED_INFO_GROUP_LABEL), 0) )
    //{
        return true;
    //}
    //return false;
}
//
//  BleedInfoPluginArtToArtboardMatcher.cpp
//  SafeguardTools
//
//  Created by T431962 on 11/15/16.
//
//

#include "BleedInfoPluginArtToArtboardMatcher.hpp"
#include "DictionaryWriter.h"
#include "SafeguardFileConstants.h"
#include <vector>
#include "ArtTree.h"
#include "BleedInfo.h"

using PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher;

BleedInfoPluginArtToArtboardMatcher::BleedInfoPluginArtToArtboardMatcher()
{
    vector<AIArtHandle> savedInDictPluginArts;
    DictionaryWriter dw;
    dw.GetVectorOfAIArtHandleFromIdentifier(savedInDictPluginArts, SG_BLEEDINFO_ARTHANDLES);
    
    if (savedInDictPluginArts.size() > 0)
    {
        sortedPluginArts = GetArtboardOfArts(savedInDictPluginArts);
        DisposePluginArtHandlesWithNoArtboard(savedInDictPluginArts);
    }
}

void BleedInfoPluginArtToArtboardMatcher::DisposePluginArtHandlesWithNoArtboard(vector<AIArtHandle> savedInDictPluginArts) const
{
    std::vector<AIArtHandle> sortedDictPa(savedInDictPluginArts);
    std::vector<AIArtHandle> sortedSpa;
    //Just get the artHandles out of our map
    for ( auto item : sortedPluginArts )
    {
        sortedSpa.push_back(item.second);
    }
    
    std::sort(sortedDictPa.begin(),sortedDictPa.end());
    std::sort(sortedSpa.begin(),sortedSpa.end());
    
    // Now that we have sorted ranges (i.e., containers), find the differences
    std::vector<AIArtHandle> vDifferences;
    
    std::set_difference(sortedDictPa.begin(),
                        sortedDictPa.end(),
                        sortedSpa.begin(),
                        sortedSpa.end(),
                        std::back_inserter(vDifferences));
    
    for ( auto item : vDifferences )
    {
        sAIArt->DisposeArt(item);
    }
}

bool BleedInfoPluginArtToArtboardMatcher::IsBleedInfoPluginArtCreated(int artboardIndex)
{
    return sortedPluginArts.count(artboardIndex);
}

AIArtHandle BleedInfoPluginArtToArtboardMatcher::GetArt(int artboardIndex)
{
    auto iter = sortedPluginArts.find(artboardIndex);
    if (iter != sortedPluginArts.end())
    {
        return iter->second;
    }
    return NULL;
}

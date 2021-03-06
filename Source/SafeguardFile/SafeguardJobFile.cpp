//
//  SafeguardJobFile.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "SafeguardJobFile.h"
#include "ArtTree.h"
#include <vector>
#include "SafeguardJobFileDTO.hpp"
#include "BleedInfoPluginArtToArtboardMatcher.hpp"

using SafeguardFile::SafeguardJobFile;
using SafeguardFile::PlateNumber;
using PlateBleedInfo::BleedInfo;

SafeguardJobFile::SafeguardJobFile(bool redrawAllWithoutCheck)
{
    for ( int i = 0; i < GetArtboardCount(); i++ )
    {
        plates.emplace(i, make_shared<Plate>(i, redrawAllWithoutCheck));
    }
}

SafeguardJobFile::SafeguardJobFile(const PlateBleedInfo::SafeguardJobFileDTO* dto, bool redrawAllWithoutCheck)
{
    for ( int i = 0; i < GetArtboardCount(); i++ )
    {
        plates.emplace(i, make_shared<Plate>(i, redrawAllWithoutCheck, &dto->GetPlateDTOs()[i]));
    }
}

void SafeguardJobFile::UpdateBleedInfo()
{
    for ( auto& plate : plates )
    {
        plate.second->Draw();
    }
}

void SafeguardJobFile::RemoveBleedInfo()
{
    for ( auto plate : plates )
    {
        plate.second->RemoveBleedInfo();
    }
}

const PlateNumber SafeguardJobFile::GetPlateNumber(int plateIndex) const
{
    auto iter = plates.find(plateIndex);
    if (iter != plates.end() )
    {
        return iter->second->GetPlateNumber();
    }
    else
    {
        return PlateNumber();
    }
}

const string SafeguardJobFile::GetToken(int plateIndex) const
{
    auto iter = plates.find(plateIndex);
    if (iter != plates.end() )
    {
        return iter->second->GetToken();
    }
    else
    {
        return "";
    }
}

AIRealRect SafeguardJobFile::GetBleeds(int plateIndex) const
{
    auto iter = plates.find(plateIndex);
    if (iter != plates.end() )
    {
        return iter->second->GetBleeds();
    }
    else
    {
        return AIRealRect{0,0,0,0};
    }
}

ColorList SafeguardJobFile::GetAllColorsOnJob() const
{
    vector<ColorList> colorLists;
    for (auto plate : plates)
    {
        colorLists.push_back(plate.second->GetColors());
    }
    for ( int i = 1; i < colorLists.size(); i++ )
    {
        colorLists[0].AddColorsToList(colorLists.at(i));
    }
    
    ColorList colorList = colorLists[0]; //Copy constructor calls duplicate removal, etc.

    return colorList;
}

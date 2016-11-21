//
//  SafeguardJobFile.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "SafeguardJobFile.h"
#include "PlateBleedInfoUIController.hpp"
#include "BtDocumentView.hpp"
#include "DictionaryWriter.h"
#include "SafeguardFileConstants.h"
#include "ArtTree.h"
#include <vector>
#include "BleedInfoPluginArtToArtboardMatcher.hpp"
#include "SafeguardJobFileDTO.hpp"

using SafeguardFile::SafeguardJobFile;
using SafeguardFile::PlateNumber;
using PlateBleedInfo::PlateBleedInfoUIController;
using PlateBleedInfo::BleedInfo;

SafeguardJobFile::SafeguardJobFile()
{
    for ( int i = 0; i < GetArtboardCount(); i++ )
    {
        plates.emplace(i, make_shared<Plate>(i));
    }
}

SafeguardJobFile::SafeguardJobFile(const PlateBleedInfo::SafeguardJobFileDTO* dto)
{
    for ( int i = 0; i < GetArtboardCount(); i++ )
    {
        plates.emplace(i, make_shared<Plate>(i, &dto->GetPlateDTOs()[i]));
    }
}

void SafeguardJobFile::UpdateBleedInfo(bool skipCheck)
{
    if (skipCheck || PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        size_t gTimeStamp = sAIArt->GetGlobalTimeStamp();
        DictionaryWriter dw;
        AIReal aTSDict = dw.GetAIRealFromIdentifier(SafeguardFile::PLATE_BLEEDINFO_TIMESTAMP);
        
        if ( gTimeStamp != aTSDict )
        {
            for ( auto& plate : plates )
            {
                plate.second->DrawBleedInfo();
            }
            
            DictionaryWriter dw;
            dw.AddAIRealToDictionary(sAIArt->GetGlobalTimeStamp(), SafeguardFile::PLATE_BLEEDINFO_TIMESTAMP);
        }
    }
}

void SafeguardJobFile::EditBleedInfo()
{
    if ( !PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        UpdateBleedInfo(true);
    }
    
    BtDocumentView docView;
    docView.StoreCurrentDocumentView();
    
    PlateBleedInfoUIController().LoadExtension();
    sAICSXSExtension->LaunchExtension(PlateBleedInfoUIController::PLATEBLEEDINFO_UI_EXTENSION);
//    sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Edit Safeguard Plate Info"), ai::UnicodeString("Redo Edit Safeguard Plate Info"));
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

SafeguardFile::ColorList SafeguardJobFile::GetAllColorsOnJob() const
{
    vector<SafeguardFile::ColorList> colorLists;
    for (auto plate : plates)
    {
        colorLists.push_back(plate.second->GetColors());
    }
    for ( int i = 1; i < colorLists.size(); i++ )
    {
        colorLists[0].AddColorsToList(colorLists.at(i));
    }
    ColorList colorList = colorLists[0];
    colorList.RemoveDuplicates();
    colorList.RemoveNonPrintingColors();
    colorList.Sort();
    return colorList;
}

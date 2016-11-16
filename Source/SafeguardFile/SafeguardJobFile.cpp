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

using SafeguardFile::SafeguardJobFile;
using SafeguardFile::PlateNumber;
using SafeguardFile::PlateBleedInfoUIController;
using SafeguardFile::BleedInfo;

SafeguardJobFile::SafeguardJobFile()
{
    for ( int i = 0; i < GetArtboardCount(); i++ )
    {
        plates.emplace(i, Plate(i));
    }
}

SafeguardJobFile::~SafeguardJobFile()
{
    vector<AIArtHandle> pluginArts;
    for ( auto p : plates )
    {
        if (p.second.GetBleedInfoPluginArtHandle())
        {
            pluginArts.push_back(p.second.GetBleedInfoPluginArtHandle());
        }
    }
    if (pluginArts.size() > 0)
    {
        DictionaryWriter dw;
        dw.AddVectorOfAIArtHandleToDictionary(pluginArts, SG_BLEEDINFO_ARTHANDLES);
    }
}

void SafeguardJobFile::PutDataInDTO(PlateBleedInfoDTO::SafeguardJobFileDTO& dto, bool fullColorName)
{
    for ( auto plate : plates )
    {
        dto.AddPlate( PlateBleedInfoDTO::PlateDTO(plate.second.BleedInfo(), fullColorName) );
    }
}

void SafeguardJobFile::LoadDataFromDTO(PlateBleedInfoDTO::SafeguardJobFileDTO dto)
{
    for ( auto platedto : dto.GetPlateDTOs() )
    {
        plates.at(platedto.artboardIndex).FillBleedInfoFromPlateDTO(&platedto);
    }
}

void SafeguardJobFile::UpdateBleedInfo(bool skipCheck)
{
    if ( skipCheck || PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        size_t gTimeStamp = sAIArt->GetGlobalTimeStamp();
        DictionaryWriter dw;
        AIReal aTSDict = dw.GetAIRealFromIdentifier(SafeguardFile::PLATE_BLEEDINFO_TIMESTAMP);
        
        if ( gTimeStamp != aTSDict )
        {
            for ( auto& plate : plates )
            {
                plate.second.DrawBleedInfo();
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
        plate.second.RemoveBleedInfo();
    }
}

vector<BleedInfo> SafeguardJobFile::GetBleedInfo() const
{
    vector<BleedInfo> bi;
    for ( auto plate : plates )
    {
        bi.push_back(plate.second.BleedInfo());
    }
    return bi;
}

const PlateNumber SafeguardJobFile::GetPlateNumber(int plateIndex) const
{
    try
    {
        return plates.at(plateIndex).GetPlateNumber();
    }
    catch (std::out_of_range e)
    {
        return PlateNumber();
    }
}

const string SafeguardJobFile::GetToken(int plateIndex) const
{
    try
    {
        return plates.at(plateIndex).GetToken();
    }
    catch (std::out_of_range e)
    {
        return "";
    }
}

AIRealRect SafeguardJobFile::GetBleeds(int plateIndex) const
{
    try
    {
        return plates.at(plateIndex).GetBleeds();
    }
    catch (std::out_of_range e)
    {
        return AIRealRect{0,0,0,0};
    }
}

SafeguardFile::ColorList SafeguardJobFile::GetAllColorsOnJob() const
{
    vector<SafeguardFile::ColorList> colorLists;
    for (auto plate : plates)
    {
        colorLists.push_back(plate.second.GetColors());
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

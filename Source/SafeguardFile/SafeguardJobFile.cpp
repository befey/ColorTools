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

using SafeguardFile::SafeguardJobFile;
using SafeguardFile::PlateNumber;
using SafeguardFile::PlateBleedInfoUIController;
using SafeguardFile::BleedInfo;

SafeguardJobFile::SafeguardJobFile()
{    
    vector<AIArtHandle> pluginArts;
    DictionaryWriter dw;
    dw.GetVectorOfAIArtHandleFromIdentifier(pluginArts, SG_BLEEDINFO_ARTHANDLES);
        
    if (pluginArts.size() > 0)
    {
        for ( auto ah : pluginArts )
        {
            int i = GetArtboardOfArt(ah);

            auto success = plates.emplace(i, Plate(i, ah));
            if (success.second == false)
            {
                throw std::runtime_error("The artboard already has a Plate constructed!");
            }
        }
    }
    else
    {
        for ( int i = 0; i < GetArtboardCount(); i++ )
        {
            plates.emplace(i, Plate(i));
        }
    }
}

SafeguardJobFile::~SafeguardJobFile()
{
    vector<AIArtHandle> pluginArts;
    for ( auto p : plates )
    {
        pluginArts.push_back(p.second.GetBleedInfoPluginArtHandle());
    }
    DictionaryWriter dw;
    dw.AddVectorOfAIArtHandleToDictionary(pluginArts, SG_BLEEDINFO_ARTHANDLES);
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

void SafeguardJobFile::UpdateBleedInfo()
{
    for ( auto& plate : plates )
    {
        plate.second.DrawBleedInfo();
    }
}

void SafeguardJobFile::EditBleedInfo()
{
    BtDocumentView docView;
    docView.StoreCurrentDocumentView();
    
    PlateBleedInfoUIController plateBleedInfoUIController;
    plateBleedInfoUIController.LoadExtension();
    sAICSXSExtension->LaunchExtension(PlateBleedInfoUIController::PLATEBLEEDINFO_UI_EXTENSION);
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
    SafeguardFile::ColorList colorList;
    for (auto plate : plates)
    {
        colorList.AddColorsToList(plate.second.GetColors());
    }
    return colorList;
}

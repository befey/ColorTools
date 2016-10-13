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
            
            if (i >= 0)
            {
                auto success = plates.emplace(i, Plate(i, ah));
                if (success.second == false)
                {
                    throw std::runtime_error("The artboard already has a Plate constructed!");
                }
            }
            else
            {
                throw std::runtime_error("Couldn't figure out which artboard the plugin art goes with!");
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
    
//    PlateBleedInfoDTO::SafeguardJobFileDTO dto;
//    if ( dto.RecallFromDocumentDictionary() )
//    {
//        LoadDataFromDTO(dto);
//    }
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

void SafeguardJobFile::LoadDataFromDTO(PlateBleedInfoDTO::SafeguardJobFileDTO dto)
{
    int size = 0;
    dto.NumPlates() >= plates.size() ? size = int(plates.size()) : size = dto.NumPlates();
   
    for ( int i = 0; i < size; i++ )
    {
        plates.at(i).BleedInfo()
            .ShouldDrawBleedInfo(dto.ShouldDrawBleedInfo(i))
            .ArtboardName(dto.ArtboardName(i))
            .ShouldAddCmykBlocks(dto.ShouldAddCmykBlocks(i))
            .TickMarkStyle(TickMarkStyle(dto.TickMarkStyle(i)));
        for ( auto color : dto.ColorList(i) )
        {
            plates.at(i).BleedInfo().ColorList().SetColorMethod(color.colorName, InkMethod(color.method) );
        }
    }
}

void SafeguardJobFile::PutDataInDTO(PlateBleedInfoDTO::SafeguardJobFileDTO& dto, bool fullColorName)
{
    for ( auto plate : plates )
    {
        PlateBleedInfoDTO::PlateDTO p;
        p.shouldDrawBleedInfo  = plate.second.BleedInfo().ShouldDrawBleedInfo();
        p.artboardIndex = plate.second.BleedInfo().ArtboardIndex();
        p.artboardName = plate.second.BleedInfo().ArtboardName(p.isDefaultArtboardName);
        p.plateNumber = plate.second.BleedInfo().PlateNumber();
        p.token = plate.second.BleedInfo().Token();
        p.shouldAddCmykBlocks = plate.second.BleedInfo().ShouldAddCmykBlocks();
        p.tmStyle = int(plate.second.BleedInfo().TickMarkSettings().TickMarkStyle());

        
        for ( auto color : plate.second.BleedInfo().ColorList().GetColorList() )
        {
            PlateBleedInfoDTO::ColorDTO c;
            c.colorName = fullColorName ? color.Name() : GetInnerPantoneColorNumber(color.Name());
            c.method = int(color.Method());
            p.c.push_back(c);
        }
        
        dto.AddPlate(p);
    }
}

void SafeguardJobFile::UpdateBleedInfo()
{
    for ( auto plate : plates )
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

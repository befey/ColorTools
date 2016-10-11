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

using SafeguardFile::SafeguardJobFile;
using SafeguardFile::PlateNumber;
using SafeguardFile::PlateBleedInfoUIController;
using SafeguardFile::BleedInfo;

SafeguardJobFile::SafeguardJobFile()
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardID count;
    sAIArtboard->GetCount(abList, count);
    
    plates.clear();
    for ( int i = 0; i < count; i++ )
    {
        plates.push_back(Plate(i));
    }
    
    PlateBleedInfoDTO::SafeguardJobFileDTO dto;
    if ( dto.RecallFromDocumentDictionary() )
    {
        LoadDataFromDTO(dto);
    }
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
        p.shouldDrawBleedInfo  = plate.BleedInfo().ShouldDrawBleedInfo();
        p.artboardIndex = plate.BleedInfo().ArtboardIndex();
        p.artboardName = plate.BleedInfo().ArtboardName(p.isDefaultArtboardName);
        p.plateNumber = plate.BleedInfo().PlateNumber();
        p.token = plate.BleedInfo().Token();
        p.shouldAddCmykBlocks = plate.BleedInfo().ShouldAddCmykBlocks();
        p.tmStyle = int(plate.BleedInfo().TickMarkSettings().TickMarkStyle());

        
        for ( auto color : plate.BleedInfo().ColorList().GetColorList() )
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
        plate.DrawBleedInfo();
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
        plate.RemoveBleedInfo();
    }
}

vector<BleedInfo> SafeguardJobFile::GetBleedInfo() const
{
    vector<BleedInfo> bi;
    for ( auto plate : plates )
    {
        bi.push_back(plate.BleedInfo());
    }
    return bi;
}

const PlateNumber SafeguardJobFile::GetPlateNumber(int plateIndex) const
{
    if ( plates.size() > plateIndex )
    {
        return plates[plateIndex].GetPlateNumber();
    }
    return PlateNumber();
}

const string SafeguardJobFile::GetToken(int plateIndex) const
{
    if ( plates.size() > plateIndex )
    {
        return plates[plateIndex].GetToken();
    }
    return "";
}

AIRealRect SafeguardJobFile::GetBleeds(int plateIndex) const
{
    if ( plates.size() > plateIndex )
    {
        return plates[plateIndex].GetBleeds();
    }
    return AIRealRect{0,0,0,0};
}

SafeguardFile::ColorList SafeguardJobFile::GetAllColorsOnJob() const
{
    SafeguardFile::ColorList colorList;
    for (auto plate : plates)
    {
        colorList.AddColorsToList(plate.GetColors());
    }
    return colorList;
}

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

using SafeguardFile::SafeguardJobFile;
using SafeguardFile::PlateNumber;
using PrintToPdf::PdfResults;
using PrintToPdf::PdfSettings;
using SafeguardFile::PlateBleedInfoUIController;
using SafeguardFile::BleedInfo;

SafeguardJobFile::SafeguardJobFile()
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardID count;
    sAIArtboard->GetCount(abList, count);
    
    plates.clear();
    for (int i = 0; i < count; i++)
    {
        plates.push_back(Plate(i));
    }
//  Don't want to create side effects here. All document changes need to be made on purpose.  
//    if (ShouldDrawBleedInfo()) {
//        AddBleedInfo();
//    }
}


void SafeguardJobFile::AddBleedInfo()
{
    //if (ShouldDrawBleedInfo())
    {
        for (auto plate : plates)
        {
            plate.DrawBleedInfo();
        }
    }
}

void SafeguardJobFile::UpdateBleedInfo()
{
    for (auto plate : plates)
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
    for (auto plate : plates)
    {
        plate.RemoveBleedInfo();
    }
}

bool SafeguardJobFile::ShouldDrawBleedInfo()
{
    //if ( CheckDictionaryForArtObjectWithIdentifier(ai::UnicodeString(SafeguardFile::PLATE_BLEED_INFO_GROUP_LABEL), 0) )
    //{
    //    return true;
    //}
    return false;
}

const BleedInfo SafeguardJobFile::GetBleedInfo(int plateIndex) const
{
    if (plates.size() > plateIndex)
    {
        return plates[plateIndex].GetBleedInfo();
    }
}

const PlateNumber SafeguardJobFile::GetPlateNumber(int plateIndex) const
{
    if (!plates.empty())
    {
        return plates[0].GetPlateNumber();
    }
    return PlateNumber();
}

const string SafeguardJobFile::GetToken(int plateIndex) const
{
    if (plates.size() > plateIndex)
    {
        return plates[plateIndex].GetToken();
    }
    return "";
}

AIRealRect SafeguardJobFile::GetBleeds(int plateIndex) const
{
    if (!plates.empty())
    {
        return plates[0].GetBleeds();
    }
    return AIRealRect{0,0,0,0};
}

vector<SafeguardFile::PlateBleedInfoDTO> SafeguardJobFile::GetPlateBleedInfoDTO() const
{
    vector<PlateBleedInfoDTO> dtos;
    for (auto plate : plates)
    {
        dtos.push_back(PlateBleedInfoDTO(plate.GetBleedInfo()));
    }
    return dtos;
}
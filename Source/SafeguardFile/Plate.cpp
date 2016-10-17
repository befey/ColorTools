//
//  Plate.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "Plate.h"
#include "SafeguardFileConstants.h"
#include "BtAteTextFeatures.h"
#include "TickMarkDrawer.h"
#include "TickMarkSettings.hpp"
#include "ColorListDrawer.h"
#include "FileNameDateDrawer.h"
#include "ATEFuncs.h"
#include "ColorFuncs.h"
#include "ArtTree.h"
#include "AIColor.h"
#include "DictionaryWriter.h"
#include "SafeguardJobFileDTO.hpp"
#include <functional>
#include <boost/filesystem/operations.hpp>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

using SafeguardFile::Plate;
using SafeguardFile::PlateNumber;
using PrintToPdf::PdfPreset;
using SafeguardFile::BleedInfo;

namespace fs = boost::filesystem;

Plate::Plate(ai::ArtboardID id, AIArtHandle pluginArtHandle)
: bleedInfo(id), bleedInfoPluginArt(pluginArtHandle)
{
    bleedInfoDrawer = make_shared<BleedInfoDrawer>(bleedInfo.ArtboardIndex());
    
    if (bleedInfoPluginArt)
    {
        DictionaryWriter dw(bleedInfoPluginArt);
        string json;
        dw.GetStringDataFromIdentifier(json, PLATE_BLEEDINFO);
        
        PlateBleedInfoDTO::PlateDTO dto(json);

        FillBleedInfoFromPlateDTO(&dto);
    }
}

const PlateNumber Plate::GetPlateNumber() const
{
    return bleedInfo.PlateNumber();
}

const string Plate::GetToken() const
{
    return bleedInfo.Token();
}

BleedInfo& Plate::BleedInfo()
{
    return bleedInfo;
}

void Plate::DrawBleedInfo()
{
    bleedInfoDrawer->AddDrawer( make_shared<TickMarkDrawer>(bleedInfo.TickMarkSettings()) );
    
    ProductType pt = bleedInfo.PlateNumber().GetProductType();
    if (pt == ProductType::BusinessStat)
    {
        bleedInfoDrawer->AddDrawer( make_shared<BusStatColorListDrawer>(bleedInfo.ArtboardBounds(), bleedInfo.ColorList()) );
        bleedInfoDrawer->AddDrawer( make_shared<BusStatFileNameDateDrawer>(bleedInfo.ArtboardBounds(), bleedInfo.PlateNumber(), bleedInfo.Token(), bleedInfo.LastModified()) );
    }
    else if (pt == ProductType::Continuous)
    {
        bleedInfoDrawer->AddDrawer( make_shared<ContinuousColorListDrawer>(bleedInfo.ArtboardBounds(), bleedInfo.ColorList()) );
        bleedInfoDrawer->AddDrawer( make_shared<ContinuousFileNameDateDrawer>(bleedInfo.ArtboardBounds(), bleedInfo.PlateNumber(), bleedInfo.Token(), bleedInfo.LastModified()) );
    }
    else if (pt == ProductType::CutSheet || pt == ProductType::Envelope)
    {
        bleedInfoDrawer->AddDrawer( make_shared<LaserColorListDrawer>(bleedInfo.ArtboardBounds(), bleedInfo.ColorList()) );
        bleedInfoDrawer->AddDrawer( make_shared<LaserFileNameDateDrawer>(bleedInfo.ArtboardBounds(), bleedInfo.PlateNumber(), bleedInfo.Token(), bleedInfo.LastModified()) );
    }

    if (ShouldDrawBleedInfo())
    {
        bleedInfoPluginArt = bleedInfoDrawer->Draw(bleedInfoPluginArt);
        WriteBleedInfoToPluginArt();
    }
    else
    {
        RemoveBleedInfo(); // bleedInfoPluginArt = NULL;
    }
}

bool Plate::ShouldDrawBleedInfo()
{
    return bleedInfo.ShouldDrawBleedInfo();
}


void Plate::RemoveBleedInfo()
{
    if (bleedInfoPluginArt)
    {
        bleedInfoDrawer->Remove(bleedInfoPluginArt); // bleedInfoPluginArt = NULL;
    }
}
                    
AIRealRect Plate::GetArtboardBounds() const
{
    return bleedInfo.ArtboardBounds();
}

tm Plate::GetLastModified() const
{
    return bleedInfo.LastModified();
}

string Plate::GetArtboardName(bool& isDefault) const
{
    return bleedInfo.ArtboardName(isDefault);
}

AIRealRect Plate::GetBleeds() const
{
    return bleedInfo.Bleeds();
}

SafeguardFile::ColorList Plate::GetColors()
{
    return bleedInfo.ColorList();
}

string Plate::GetBleedInfoAsJson(bool fullColorName) const
{
    std::ostringstream os;
    {
        PlateBleedInfoDTO::PlateDTO pdto(bleedInfo, fullColorName);
        cereal::JSONOutputArchive oarchive(os); // Create an output archive
        oarchive(pdto);
    }
    return os.str();
}

void Plate::FillBleedInfoFromPlateDTO(PlateBleedInfoDTO::PlateDTO* dto)
{
    BleedInfo()
    .ShouldDrawBleedInfo(dto->shouldDrawBleedInfo)
    .ArtboardName(dto->artboardName)
    .ShouldAddCmykBlocks(dto->shouldAddCmykBlocks)
    .TickMarkStyle(TickMarkStyle(dto->tmStyle));
    for ( auto color : dto->c )
    {
        BleedInfo().ColorList().SetColorMethod(color.colorName, InkMethod(color.method) );
    }
    WriteBleedInfoToPluginArt();
}

void Plate::WriteBleedInfoToPluginArt()
{
    if (bleedInfoPluginArt)
    {
        DictionaryWriter dw(bleedInfoPluginArt);
        dw.AddStringDataToDictionary(GetBleedInfoAsJson(false), PLATE_BLEEDINFO);
    }
}

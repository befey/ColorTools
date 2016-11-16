//
//  Plate.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "Plate.h"
#include "SafeguardFileConstants.h"
#include "TickMarkSettings.hpp"
#include "DictionaryWriter.h"
#include "SafeguardJobFileDTO.hpp"
#include "BleedInfoPluginArtToArtboardMatcher.hpp"
#include <boost/filesystem/operations.hpp>
#include <ctime>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

using SafeguardFile::Plate;
using SafeguardFile::PlateNumber;
using PrintToPdf::PdfPreset;
using SafeguardFile::BleedInfo;

namespace fs = boost::filesystem;

Plate::Plate(ai::ArtboardID id)
: bleedInfo(id)
{
    bleedInfoDrawer = make_shared<BleedInfoDrawer>(bleedInfo.ArtboardIndex());
    
    bleedInfoPluginArt = PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().GetArt(id);
    
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
    bleedInfoDrawer->AddDrawer( bleedInfoDrawer->MakeTickMarkDrawer(bleedInfo.TickMarkSettings()) );
    
    ProductType pt = bleedInfo.PlateNumber().GetProductType();
    
    bleedInfoDrawer->AddDrawer( bleedInfoDrawer->MakeColorListDrawer(pt, bleedInfo.ArtboardBounds(), bleedInfo.ColorList()) );
    bleedInfoDrawer->AddDrawer( bleedInfoDrawer->MakeFileNameDateDrawer(pt, bleedInfo.ArtboardBounds(), bleedInfo.PlateNumber(), bleedInfo.Token(), bleedInfo.LastModified()) );
    
    if (bleedInfo.ShouldAddCmykBlocks())
    {
        AIRealRect abBounds = bleedInfo.ArtboardBounds();
        AIRealRect bounds = { //CMYK Blocks are 325x25px
            .left = abBounds.left + ((abBounds.right - abBounds.left)/2) - (325/2),
            .top = abBounds.top + 5 + 25,
            .right = abBounds.right - ((abBounds.right - abBounds.left)/2) + (325/2),
            .bottom = abBounds.top + 5
        };
        
        bleedInfoDrawer->AddDrawer( bleedInfoDrawer->MakeSgSymbolDrawer(bounds, AI_CMYK_BLOCKS) );
    }

    if (bleedInfo.PlateNumber().GetProductType() == Continuous)
    {
        AIRealRect abBounds = bleedInfo.ArtboardBounds();
        AIRealRect bounds = { //Reg block is 24.3x36px
            .left = abBounds.left,
            .top = abBounds.top - 42,
            .right = abBounds.left + 24.3,
            .bottom = abBounds.top - 42 - 36
        };
        
        bleedInfoDrawer->AddDrawer( bleedInfoDrawer->MakeSgSymbolDrawer(bounds, AI_CONTINUOUS_REG_TARGET) );
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
    //.ArtboardName(dto->artboardName) //Do not set artboard name here or we'll overwrite what's been set in artboards panel.
    //.ShouldAddCmykBlocks(dto->shouldAddCmykBlocks) //Do not set cmyk blocks here or we'll overwrite what's been set by the file type and color list
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

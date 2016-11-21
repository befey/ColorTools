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
#include <boost/filesystem/operations.hpp>
#include <ctime>
#include "BleedInfoWriter.hpp"
#include "BleedInfoPluginArtToArtboardMatcher.hpp"

using SafeguardFile::Plate;
using SafeguardFile::PlateNumber;
using PrintToPdf::PdfPreset;
using PlateBleedInfo::BleedInfo;

namespace fs = boost::filesystem;

Plate::Plate(ai::ArtboardID id)
: bleedInfo(id)
{
}

Plate::Plate(ai::ArtboardID id, const PlateBleedInfo::PlateDTO* dto) : Plate(id)
{
    bleedInfo.FillBleedInfoFromPlateDTO(dto);
}
    
const PlateNumber Plate::GetPlateNumber() const
{
    return bleedInfo.PlateNumber();
}

const string Plate::GetToken() const
{
    return bleedInfo.Token();
}

void Plate::DrawBleedInfo()
{
    bleedInfo.Draw();
}

void Plate::RemoveBleedInfo()
{
    bleedInfo.Remove();
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

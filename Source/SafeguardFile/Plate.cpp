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
#include <functional>
#include <boost/filesystem/operations.hpp>

using SafeguardFile::Plate;
using SafeguardFile::PlateNumber;
using PrintToPdf::PdfPreset;
using SafeguardFile::BleedInfo;

namespace fs = boost::filesystem;

Plate::Plate(ai::ArtboardID id)
: bleedInfo(id)
{
    bleedInfoDrawer = make_shared<BleedInfoDrawer>(bleedInfo.ArtboardIndex());
    
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
}

const PlateNumber Plate::GetPlateNumber() const
{
    return bleedInfo.PlateNumber();
}

const string Plate::GetToken() const
{
    return bleedInfo.Token();
}

const BleedInfo Plate::GetBleedInfo() const
{
    return bleedInfo;
}

void Plate::DrawBleedInfo()
{
    bleedInfoDrawer->Draw();
}

void Plate::RemoveBleedInfo()
{
    unique_ptr<DictionaryWriter> dw = make_unique<DictionaryWriter>();
    AIArtHandle foundArt = dw->GetArtHandleFromIdentifier(PLATE_BLEED_INFO_GROUP_LABEL, bleedInfo.ArtboardIndex());
    if (foundArt)
    {
        bleedInfoDrawer->Remove(foundArt);
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
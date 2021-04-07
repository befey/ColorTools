//
//  Plate.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "Plate.h"

using SafeguardFile::Plate;
using SafeguardFile::PlateNumber;

Plate::Plate(ai::ArtboardID id, bool redrawAllWithoutCheck, const PlateBleedInfo::PlateDTO* dto)
: bleedInfo(make_shared<PlateBleedInfo::BleedInfo>(id, redrawAllWithoutCheck, dto))
{
}
    
const PlateNumber Plate::GetPlateNumber() const
{
    return bleedInfo->PlateNumber();
}

const string Plate::GetToken() const
{
    return bleedInfo->Token();
}
               
AIRealRect Plate::GetArtboardBounds() const
{
    return bleedInfo->ArtboardBounds();
}

tm Plate::GetLastModified() const
{
    return bleedInfo->LastModified();
}

string Plate::GetArtboardName(bool& isDefault) const
{
    return bleedInfo->ArtboardName(isDefault);
}

AIRealRect Plate::GetBleeds() const
{
    return bleedInfo->Bleeds();
}

ColorList Plate::GetColors()
{
    return bleedInfo->ColorList();
}

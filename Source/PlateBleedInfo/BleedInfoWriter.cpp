//
//  BleedInfoWriter.cpp
//  SafeguardTools
//
//  Created by T431962 on 11/16/16.
//
//

#include "BleedInfoWriter.hpp"
#include "DictionaryWriter.h"
#include "SafeguardJobFileDTO.hpp"

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

using PlateBleedInfo::BleedInfoToArtStrategy;
using PlateBleedInfo::BleedInfoToJsonStrategy;

void BleedInfoToArtStrategy::Write(const PlateBleedInfo::BleedInfo* bleedInfo) const
{
    if (art != NULL)
    {
        DictionaryWriter dw(art);
        string json;
        BleedInfoWriter(bleedInfo, make_shared<BleedInfoToJsonStrategy>(json, true));
        dw.AddStringDataToDictionary(json, SafeguardFile::PLATE_BLEEDINFO);
    }
}

void BleedInfoToJsonStrategy::Write(const PlateBleedInfo::BleedInfo* bleedInfo) const
{
    std::ostringstream os;
    {
        PlateBleedInfo::PlateDTO pdto(bleedInfo, fullColorName);
        cereal::JSONOutputArchive oarchive(os); // Create an output archive
        oarchive(pdto);
    }
    json = os.str();
}

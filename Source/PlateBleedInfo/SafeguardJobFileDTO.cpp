//
//  SafeguardJobFileDTO.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/22/16.
//
//

#include "SafeguardJobFileDTO.hpp"
#include "DictionaryWriter.h"
#include "BleedInfo.h"
#include "SafeguardJobFile.h"
#include "ColorList.h"
#include "ArtTree.h"
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

using PlateBleedInfo::SafeguardJobFileDTO;
using PlateBleedInfo::PlateDTO;
using PlateBleedInfo::ColorDTO;

ColorDTO::ColorDTO(BtColor color, bool fullColorName)
:
method(int(color.Method()))
{
    colorName = fullColorName ? color.Name() : GetInnerPantoneColorNumber(color.Name());
}

PlateDTO::PlateDTO(const PlateBleedInfo::BleedInfo* bleedInfo, const bool fullColorName)
:
shouldDrawBleedInfo(bleedInfo->ShouldDrawBleedInfo()),
shouldPrint(bleedInfo->ShouldPrint()),
artboardIndex(bleedInfo->ArtboardIndex()),
plateNumber(bleedInfo->PlateNumber()),
token(bleedInfo->Token()),
shouldAddCmykBlocks(bleedInfo->ShouldAddCmykBlocks()),
tmStyle(int(bleedInfo->TickMarkSettings().TickMarkStyle()))
{
    artboardName = bleedInfo->ArtboardName(isDefaultArtboardName);
    
    for (auto color : bleedInfo->ConstColorList() )
    {
        c.push_back(ColorDTO(color, fullColorName));
    }
}

PlateDTO::PlateDTO(const string jsonBleedInfo)
{
    std::istringstream is(jsonBleedInfo);
    {
        try
        {
            cereal::JSONInputArchive iarchive(is); // Create an input archive
            iarchive(*this);
        }
        catch (std::runtime_error e)
        {
            string s(e.what());
        }
    }
}

SafeguardJobFileDTO::SafeguardJobFileDTO(SafeguardFile::SafeguardJobFile* sgJobFile, bool fullColorName)
{
    for ( auto plate : sgJobFile->GetPlates() )
    {
        AddPlate( PlateBleedInfo::PlateDTO(&(plate.second->GetBleedInfo()), fullColorName) );
    }
}


void SafeguardJobFileDTO::WriteToDocumentDictionary()
{
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oarchive(ss); // Create an output archive
        oarchive(*this);
    }

    DictionaryWriter dw;
    dw.AddStringDataToDictionary(ss.str(), JOBFILE_DTO_ID);
}

bool SafeguardJobFileDTO::RecallFromDocumentDictionary()
{
    DictionaryWriter dw;
    string data;
    if ( dw.GetStringDataFromIdentifier(data, JOBFILE_DTO_ID) )
    {
        std::istringstream is(data);
        {
            cereal::JSONInputArchive iarchive(is); // Create an input archive
            try
            {
                iarchive(*this);
            }
            catch (std::runtime_error e)
            {
                return false;
            }
        }
        
        return true;
    }
    return false;
}

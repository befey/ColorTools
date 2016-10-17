//
//  SafeguardJobFileDTO.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/22/16.
//
//

#include "SafeguardJobFileDTO.hpp"
#include "DictionaryWriter.h"
#include "SafeguardJobFile.h"
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

using PlateBleedInfoDTO::SafeguardJobFileDTO;
using PlateBleedInfoDTO::PlateDTO;
using PlateBleedInfoDTO::ColorDTO;

ColorDTO::ColorDTO(BtColor color, bool fullColorName)
:
method(int(color.Method()))
{
    colorName = fullColorName ? color.Name() : GetInnerPantoneColorNumber(color.Name());
}

PlateDTO::PlateDTO(SafeguardFile::BleedInfo bleedInfo, bool fullColorName)
:
shouldDrawBleedInfo(bleedInfo.ShouldDrawBleedInfo()),
artboardIndex(bleedInfo.ArtboardIndex()),
plateNumber(bleedInfo.PlateNumber()),
token(bleedInfo.Token()),
shouldAddCmykBlocks(bleedInfo.ShouldAddCmykBlocks()),
tmStyle(int(bleedInfo.TickMarkSettings().TickMarkStyle()))
{
    artboardName = bleedInfo.ArtboardName(isDefaultArtboardName);
    
    for (auto color : bleedInfo.ColorList().GetColorList())
    {
        c.push_back(ColorDTO(color, fullColorName));
    }
}

PlateDTO::PlateDTO(string jsonBleedInfo)
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

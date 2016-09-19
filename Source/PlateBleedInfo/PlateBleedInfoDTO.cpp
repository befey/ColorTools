//
//  PlateBleedInfoDTO.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/22/16.
//
//

#include "PlateBleedInfoDTO.hpp"
#include "DictionaryWriter.h"
#include "SafeguardJobFile.h"
#include "cereal/archives/json.hpp"

using SafeguardFile::PlateBleedInfoDTO;

void PlateBleedInfoDTO::WriteToDocumentDictionary()
{
    std::stringstream ss;
    {
        cereal::JSONOutputArchive oarchive(ss); // Create an output archive
        oarchive(*this);
    }

    DictionaryWriter dw;
    dw.AddStringDataToDictionary(ss.str(), PLATEBLEEDINFO_DTO_ID);
}

bool PlateBleedInfoDTO::RecallFromDocumentDictionary()
{
    DictionaryWriter dw;
    string data;
    if ( dw.GetStringDataFromIdentifier(data, PLATEBLEEDINFO_DTO_ID) )
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
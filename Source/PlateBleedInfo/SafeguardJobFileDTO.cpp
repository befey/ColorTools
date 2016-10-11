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

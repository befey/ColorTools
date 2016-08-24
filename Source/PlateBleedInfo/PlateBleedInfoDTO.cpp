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
#include "cereal/archives/binary.hpp"

using SafeguardFile::PlateBleedInfoDTO;

PlateBleedInfoDTO::PlateBleedInfoDTO()
{
    SafeguardJobFile sgJobFile;
    
    for (int i = 0; i < sgJobFile.GetNumPlates(); i++)
    {
        BleedInfo bi = sgJobFile.GetBleedInfo(i);
        
        bool dan;
        string n = bi.ArtboardName(dan);
        
        vector<Plate::Color> colors;
        for(auto col : bi.ColorList().GetColorList())
        {
            std::string nm;
            if (ColorIsPantone(col.AiColor()))
            {
                nm = GetInnerPantoneColorNumber(col.Name());
            }
            else
            {
                nm = col.Name();
            }

            int mtd = int(col.Method());
            
            colors.push_back(Plate::Color{.colorName = nm, .method = mtd});
        }
        
        Plate p
        {
            .artboardIndex = i,
            .artboardName = n,
            .isDefaultArtboardName = dan,
            .c = colors,
            .plateNumber = string(bi.PlateNumber()),
            .token = bi.Token(),
            .shouldAddCmykBlocks = bi.AddCmykBlocks(),
            .tmStyle = int(bi.TickMarkSettings().TickMarkStyle())
        };
        
        plates.push_back(p);
    }
}

void PlateBleedInfoDTO::WriteToDocumentDictionary()
{
    std::ostringstream os(std::stringstream::binary);
    {
        cereal::BinaryOutputArchive oarchive(os); // Create an output archive
        oarchive(*this);
    }
    
    string binaryoutput = os.str();
    size_t size = binaryoutput.length();
    DictionaryWriter dw;
    dw.AddBinaryDataToDictionary((void*)binaryoutput.c_str(), size + 1, PLATEBLEEDINFO_DTO_ID);
}

void PlateBleedInfoDTO::RecallFromDocumentDictionary()
{
    DictionaryWriter dw;
    char* data;
    size_t size;
    dw.GetBinaryDataFromIdentifier((void*)data, &size, PLATEBLEEDINFO_DTO_ID);
    
    assert(size != 0);
    
    string s(data, size);
    
    std::istringstream is(s, std::stringstream::binary);

    {
        cereal::BinaryInputArchive iarchive(is); // Create an input archive
        iarchive(*this);
    }
}
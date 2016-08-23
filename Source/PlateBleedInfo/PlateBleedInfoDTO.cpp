//
//  PlateBleedInfoDTO.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/22/16.
//
//

#include "PlateBleedInfoDTO.hpp"

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
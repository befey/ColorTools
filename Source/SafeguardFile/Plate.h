//
//  Plate.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__Plate__
#define __SafeguardTools__Plate__

#include "PlateNumber.h"
#include "BleedInfo.h"
#include "PrintToPdfConstants.h"
#include "ColorList.h"
#include "AIFolders.h"
#include "SafeguardJobFileDTO.hpp"

extern AIFoldersSuite* sAIFolders;

namespace SafeguardFile
{    
    class Plate
    {
    public:
        Plate(ai::ArtboardID id);
        Plate(ai::ArtboardID id, const PlateBleedInfo::PlateDTO* dto);
        
        AIRealRect GetArtboardBounds() const;
        tm GetLastModified() const;
        
        string GetArtboardName(bool& isDefault) const;
        const ai::ArtboardID GetArtboardIndex() const { return bleedInfo.ArtboardIndex(); };
        const PlateNumber GetPlateNumber() const;
        const string GetToken() const;
        
        AIRealRect GetBleeds() const;
        SafeguardFile::ColorList GetColors();
                
        void DrawBleedInfo(bool skipCheck = false);
        void RemoveBleedInfo();
        
        const PlateBleedInfo::BleedInfo& GetBleedInfo() const { return bleedInfo; };
    private:
        PlateBleedInfo::BleedInfo bleedInfo;
    };
}
#endif /* defined(__SafeguardTools__Plate__) */

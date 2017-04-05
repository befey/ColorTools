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
#include "ColorList.h"
#include "SafeguardJobFileDTO.hpp"
#include "IDrawable.hpp"

namespace SafeguardFile
{    
    class Plate : public IDrawable
    {
    public:
        Plate(ai::ArtboardID id) : Plate(id, nullptr) {};
        Plate(ai::ArtboardID id, const PlateBleedInfo::PlateDTO* dto);
        
        AIRealRect GetArtboardBounds() const;
        tm GetLastModified() const;
        
        string GetArtboardName(bool& isDefault) const;
        const ai::ArtboardID GetArtboardIndex() const { return bleedInfo->ArtboardIndex(); };
        const PlateNumber GetPlateNumber() const;
        const string GetToken() const;
        
        AIRealRect GetBleeds() const;
        ColorList GetColors();
        
        AIArtHandle Draw(AIArtHandle existingArt = nullptr) override { return bleedInfo->Draw(existingArt); };
        void RemoveBleedInfo() { bleedInfo->Remove(); };
        
        const PlateBleedInfo::BleedInfo& GetBleedInfo() const { return *bleedInfo; };
        
    private:
        shared_ptr<PlateBleedInfo::BleedInfo> bleedInfo;
    };
}
#endif /* defined(__SafeguardTools__Plate__) */

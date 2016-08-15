//
//  Plate.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__Plate__
#define __SafeguardTools__Plate__

#include "AIPathStyle.h"
#include "AIFont.h"
#include "AIArtboard.h"
#include "AIDocument.h"
#include "PlateNumber.h"
#include "BleedInfo.h"
#include "PrintToPdfConstants.h"
#include "ColorList.h"
#include "BleedInfoDrawer.h"
#include <vector>
#include <ctime>
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"

extern AIPathStyleSuite* sAIPathStyle;
extern AIFontSuite* sAIFont;
extern AIArtboardSuite* sAIArtboard;
extern AIDocumentSuite* sAIDocument;

namespace SafeguardFile
{    
    class Plate
    {
    public:
        Plate(ai::ArtboardID id);
        //Plate(ai::ArtboardID id, string pn);
        
        AIRealRect GetArtboardBounds() const;
        tm GetLastModified() const;
        
        string GetArtboardName(AIBoolean& isDefault) const;
        const ai::ArtboardID GetArtboardIndex() const { return bleedInfo.artboardIndex; };
        const PlateNumber GetPlateNumber() const;
        const string GetToken() const;
        
        void SetPlateNumber();
        
        AIRealRect GetBleeds() const;
        
        const BleedInfo GetBleedInfo() const;
        void AddBleedInfo();
        void RemoveBleedInfo();
    private:
        BleedInfo bleedInfo;
        shared_ptr<SafeguardFile::BleedInfoDrawer> bleedInfoDrawer;
        
        void SetPlateNumber(string pn);
        string CreateToken() const;
        
        void AddColorsOfArtToColorList(AIArtHandle art);
        void FillColorList();
        
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar( CEREAL_NVP(bleedInfo) );
        }
    };
}
#endif /* defined(__SafeguardTools__Plate__) */

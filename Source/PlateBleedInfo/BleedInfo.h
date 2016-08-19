//
//  BleedInfo.h
//  SafeguardTools
//
//  Created by T431962 on 6/7/16.
//
//

#ifndef __SafeguardTools__BleedInfo__
#define __SafeguardTools__BleedInfo__

#include "AIUser.h"
#include "ColorList.h"
#include "PlateNumber.h"
#include "TickMarkSettings.hpp"
#include <ctime>
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"
#include "AIDocument.h"
#include "AIArtboard.h"

extern AIDocumentSuite* sAIDocument;
extern AIArtboardSuite* sAIArtboard;

namespace SafeguardFile
{
    class BleedInfo
    {
    public:
        BleedInfo(ai::ArtboardID artboardIndex);
        
        ai::ArtboardID ArtboardIndex() const { return artboardIndex; };
        AIRealRect ArtboardBounds() const;
        tm LastModified() const;
        
        const TickMarkSettings TickMarkSettings() const { return tmSettings; };
        
        string Token() const;
        BleedInfo& Token(string newVal);
        string ArtboardName(bool& isDefault) const;
        BleedInfo& ArtboardName(string newVal);
        
        const SafeguardFile::PlateNumber PlateNumber() const { return plateNumber; };
        const SafeguardFile::ColorList ColorList() const { return colorList; };
        AIRealRect Bleeds() const;
        
    private:
        ai::ArtboardID artboardIndex;
        SafeguardFile::ColorList colorList;
        SafeguardFile::PlateNumber plateNumber;
        bool shouldAddCMYKBlocks;
        SafeguardFile::TickMarkSettings tmSettings;
        
        void AddColorsOfArtToColorList(AIArtHandle art);
        void FillColorList();
        
        void SetPlateNumber();
        void SetPlateNumber(string pn);
        
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            bool isDefaultArtboardName;
            string artboardName = ArtboardName(isDefaultArtboardName);
            ar(CEREAL_NVP(artboardIndex),
               CEREAL_NVP(artboardName),
               CEREAL_NVP(isDefaultArtboardName),
               CEREAL_NVP(colorList),
               cereal::make_nvp("plateNumber", string(plateNumber)),
               cereal::make_nvp("token", Token()),
               CEREAL_NVP(shouldAddCMYKBlocks),
               cereal::make_nvp("tickStyle", tmSettings.TickMarkStyle())
               );
        }
    };
}

#endif /* defined(__SafeguardTools__BleedInfo__) */
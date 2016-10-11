//
//  PlateBleedInfoDTO.hpp
//  SafeguardTools
//
//  Created by T431962 on 8/22/16.
//
//

#ifndef PlateBleedInfoDTO_hpp
#define PlateBleedInfoDTO_hpp

#include "BleedInfo.h"
#include "BtColor.h"
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"

namespace PlateBleedInfoDTO
{
    class ColorDTO
    {
    public:
        ColorDTO() {};
        ColorDTO(BtColor color);
        
        string colorName;
        int method;
        
    private:
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(colorName),
               CEREAL_NVP(method)
               );
        }
    };
    
    class PlateDTO
    {
    public:
        PlateDTO() {};
        PlateDTO(SafeguardFile::BleedInfo bleedInfo);
        
        bool shouldDrawBleedInfo;
        int artboardIndex;
        string artboardName;
        bool isDefaultArtboardName;
        vector<ColorDTO> c;
        string plateNumber;
        string token;
        bool shouldAddCmykBlocks;
        int tmStyle;
        
    private:
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(shouldDrawBleedInfo),
               CEREAL_NVP(artboardIndex),
               CEREAL_NVP(artboardName),
               CEREAL_NVP(isDefaultArtboardName),
               CEREAL_NVP(c),
               CEREAL_NVP(plateNumber),
               CEREAL_NVP(token),
               CEREAL_NVP(shouldAddCmykBlocks),
               CEREAL_NVP(tmStyle)
               );
        }
    };
    
    class SafeguardJobFileDTO
    {
    public:
        SafeguardJobFileDTO() {};
        
        int NumPlates() const { return int(plates.size()); };
        void AddPlate(PlateDTO p) { plates.push_back(p); };
        
        bool ShouldDrawBleedInfo(int index) const { return plates.at(index).shouldDrawBleedInfo; };
        string ArtboardName(int index) const { return plates.at(index).artboardName; };
        bool IsDefaultArtboardName(int index) const { return plates.at(index).isDefaultArtboardName; };
        vector<ColorDTO> ColorList(int index) const { return plates.at(index).c; };
        string PlateNumber(int index) const { return plates.at(index).plateNumber; };
        string Token(int index) const { return plates.at(index).token; };
        bool ShouldAddCmykBlocks(int index) const { return plates.at(index).shouldAddCmykBlocks; };
        int TickMarkStyle(int index) const { return plates.at(index).tmStyle; };
        
        void WriteToDocumentDictionary();
        bool RecallFromDocumentDictionary();
        
    private:
        static constexpr auto JOBFILE_DTO_VERSION =  1.0;
        static constexpr auto JOBFILE_DTO_ID =       "safeguard_jobfile_dto";
        
        vector<PlateDTO> plates;
        
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar( CEREAL_NVP(plates) );
        }
    };
}

#endif /* PlateBleedInfoDTO_hpp */

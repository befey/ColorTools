//
//  PlateBleedInfoDTO.hpp
//  SafeguardTools
//
//  Created by T431962 on 8/16/16.
//
//

#ifndef PlateBleedInfoDTO_hpp
#define PlateBleedInfoDTO_hpp

#include "BleedInfo.h"
#include "Plate.h"
#include <vector>
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"
#include "cereal/types/vector.hpp"

namespace SafeguardFile
{
    class PlateBleedInfoDTO
    {
    public:
        PlateBleedInfoDTO(Plate plate)
        {
            bleedInfo = plate.GetBleedInfo();
            artboardName = plate.GetArtboardName(isDefaultArtboardName);
        };
        
    private:
        BleedInfo bleedInfo;
        string artboardName;
        bool isDefaultArtboardName;
        
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar( CEREAL_NVP(bleedInfo),
               CEREAL_NVP(artboardName),
               CEREAL_NVP(isDefaultArtboardName));
        }
    };
}

#endif /* PlateBleedInfoDTO_hpp */

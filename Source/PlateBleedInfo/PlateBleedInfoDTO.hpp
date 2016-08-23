//
//  PlateBleedInfoDTO.hpp
//  SafeguardTools
//
//  Created by T431962 on 8/22/16.
//
//

#ifndef PlateBleedInfoDTO_hpp
#define PlateBleedInfoDTO_hpp

#include "SafeguardJobFile.h"
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"
#include "cereal/types/vector.hpp"

namespace SafeguardFile
{
    class PlateBleedInfoDTO
    {
    public:
        PlateBleedInfoDTO();
        
    private:
        struct Plate
        {
            struct Color
            {
                string colorName;
                int method;
                
                template <class Archive>
                void serialize(Archive& ar)
                {
                    ar(CEREAL_NVP(colorName),
                       CEREAL_NVP(method)
                       );
                }
            };
            
            int artboardIndex;
            string artboardName;
            bool isDefaultArtboardName;
            vector<Color> c;
            string plateNumber;
            string token;
            bool shouldAddCmykBlocks;
            int tmStyle;
            
            template <class Archive>
            void serialize(Archive& ar)
            {
                ar(CEREAL_NVP(artboardIndex),
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
        
        vector<Plate> plates;
        
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar( CEREAL_NVP(plates) );
        }

    };
}

#endif /* PlateBleedInfoDTO_hpp */

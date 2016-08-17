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
#include <ctime>
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"

namespace SafeguardFile
{
    class BleedInfo
    {
    public:
        ai::ArtboardID artboardIndex;
        AIRealRect rect;
        ColorList colorList;
        PlateNumber plateNumber;
        string token;
        tm lastModified;
        bool ShouldAddCMYKBlocks;
    
    private:
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(CEREAL_NVP(artboardIndex),
               CEREAL_NVP(colorList),
               cereal::make_nvp("plateNumber", string(plateNumber)),
               CEREAL_NVP(token),
               CEREAL_NVP(ShouldAddCMYKBlocks)
               );
        }
    };
}

#endif /* defined(__SafeguardTools__BleedInfo__) */

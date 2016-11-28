//
//  BleedInfoController.hpp
//  SafeguardTools
//
//  Created by T431962 on 11/28/16.
//
//

#ifndef BleedInfoController_hpp
#define BleedInfoController_hpp

#include "AICSXSExtension.h"
#include "AIArt.h"

extern AICSXSExtensionSuite* sAICSXSExtension;
extern AIArtSuite* sAIArt;

namespace PlateBleedInfo
{
    constexpr auto PLATE_BLEEDINFO_TIMESTAMP =  "__plate_bleedinfo_timestamp__";
    
    class BleedInfoController
    {
    public:        
        void HandleCropAreaNotification();
        void HandleCreateMenu();
        void HandleEditMenu();
    private:
        bool ShouldDoUpdate();
    };
}

#endif /* BleedInfoController_hpp */

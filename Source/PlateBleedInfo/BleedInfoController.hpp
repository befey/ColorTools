//
//  BleedInfoController.hpp
//  SafeguardTools
//
//  Created by T431962 on 11/28/16.
//
//

#ifndef BleedInfoController_hpp
#define BleedInfoController_hpp

namespace PlateBleedInfo
{
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

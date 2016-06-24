//
//  BleedTextInfoDrawer.h
//  SafeguardTools
//
//  Created by T431962 on 6/24/16.
//
//

#ifndef __SafeguardTools__BleedTextInfoDrawer__
#define __SafeguardTools__BleedTextInfoDrawer__

#include "BtAteTextFeatures.h"

namespace SafeguardFile
{
    class BleedInfo;
    
    class BleedTextInfoDrawer
    {
    public:
        virtual AIArtHandle Draw() = 0;
        
    protected:
        BleedTextInfoDrawer(shared_ptr<BleedInfo> info) : p_BleedInfo(info) {};

        shared_ptr<BleedInfo> p_BleedInfo;
    };
}

#endif /* defined(__SafeguardTools__BleedTextInfoDrawer__) */

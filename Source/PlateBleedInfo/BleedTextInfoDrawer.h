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
#include "IDrawer.h"

namespace SafeguardFile
{
    class BleedInfo;
    
    class BleedTextInfoDrawer : public IDrawer
    {
    protected:
        BleedTextInfoDrawer(AIRealRect bounds, AIRealPoint anchor) : IDrawer(bounds), anchor(anchor) {};
        ATE::ITextRange SetupTextRange(AIArtHandle resultGroup, AITextOrientation orientation, AIArtHandle *newTextFrame) const;
        
        AIRealPoint anchor;
    };
}

#endif /* defined(__SafeguardTools__BleedTextInfoDrawer__) */

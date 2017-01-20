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
        BleedTextInfoDrawer(AIRealRect artboardBounds, AIRealPoint anchor) : IDrawer(artboardBounds), anchor(anchor) {};
        ATE::ITextRange SetupTextRange(AIArtHandle resultGroup, AIReal width, ATE::ParagraphJustification justification, AITextOrientation orientation, AIArtHandle *newTextFrame) const;
        
        AIRealPoint anchor;
        AIReal maxWidth;
        AIReal maxHeight;
    };
}

#endif /* defined(__SafeguardTools__BleedTextInfoDrawer__) */

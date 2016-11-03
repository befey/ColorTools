//
//  PlacedArtFileDrawer.hpp
//  SafeguardTools
//
//  Created by T431962 on 11/1/16.
//
//

#ifndef PlacedArtFileDrawer_hpp
#define PlacedArtFileDrawer_hpp

#include "IDrawer.h"
#include "BtTransformArt.hpp"
#include "AIArt.h"
#include "AIPlaced.h"
#include "AIHardSoft.h"
#include "AIRealMath.h"
#include "AIArtSet.h"

extern AIArtSuite* sAIArt;
extern AIPlacedSuite* sAIPlaced;
extern AIHardSoftSuite* sAIHardSoft;
extern AIRealMathSuite* sAIRealMath;
extern AIArtSetSuite* sAIArtSet;

namespace SafeguardFile
{
    class PlacedArtFileDrawer : public IDrawer
    {
    public:
        PlacedArtFileDrawer(AIRealRect bounds, ai::FilePath path) : bounds(bounds), path(path) {};
        
    private:
        const AIRealRect bounds;
        const ai::FilePath path;
        
        AIArtHandle DoDraw(AIArtHandle resultGroup) const override;
    };
}


#endif /* PlacedArtFileDrawer_hpp */

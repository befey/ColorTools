//
//  BleedTextInfoDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/24/16.
//
//

#include "BleedTextInfoDrawer.h"
#include "SafeguardFileConstants.h"
#include "BtLayer.hpp"

using SafeguardFile::BleedTextInfoDrawer;

ATE::ITextRange BleedTextInfoDrawer::SetupTextRange(AITextOrientation orientation, AIArtHandle *newTextFrame) const
{
    BtLayer foregroundLayer(FOREGROUND_LAYER);
    AIArtHandle prep = foregroundLayer.GetLayerGroupArt();
    
    sAITextFrame->NewPointText(kPlaceInsideOnTop, prep, orientation, anchor, newTextFrame);
    //Create the ATE range
    ATE::TextRangeRef colorListTextRangeRef;
    sAITextFrame->GetATETextRange(*newTextFrame, &colorListTextRangeRef);
    ATE::ITextRange colorListTextRange(colorListTextRangeRef);
    colorListTextRange.Remove();
    
    return colorListTextRange;
}
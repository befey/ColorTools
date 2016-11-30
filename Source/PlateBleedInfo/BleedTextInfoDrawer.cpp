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

ATE::ITextRange BleedTextInfoDrawer::SetupTextRange(AIArtHandle resultGroup, AITextOrientation orientation, AIArtHandle *newTextFrame) const
{
    sAITextFrame->NewPointText(kPlaceInsideOnTop, resultGroup, orientation, anchor, newTextFrame);
    //Create the ATE range
    ATE::TextRangeRef textRangeRef;
    sAITextFrame->GetATETextRange(*newTextFrame, &textRangeRef);
    ATE::ITextRange textRange(textRangeRef);
    textRange.Remove();
    
    return textRange;
}

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
#include "ArtTree.h"

using SafeguardFile::BleedTextInfoDrawer;

ATE::ITextRange BleedTextInfoDrawer::SetupTextRange(AIArtHandle resultGroup, AIReal width, ATE::ParagraphJustification justification, AITextOrientation orientation, AIArtHandle *newTextFrame) const
{
    AIRealRect rect = {.top = anchor.v};
    
    if (justification == ATE::kRightJustify || justification == ATE::kFullJustifyLastLineRight)
    {
        rect.left = anchor.h - width;
        rect.right = anchor.h;
    }
    else
    {
        rect.left = anchor.h;
        rect.right = anchor.h + width;
    }
    rect.bottom = anchor.v - 400;
    
    AIArtHandle path = DrawRectangle(rect, resultGroup);
    sAITextFrame->NewInPathText(kPlaceInsideOnTop, resultGroup, orientation, path, NULL, false, newTextFrame);
    //sAITextFrame->NewPointText(kPlaceInsideOnTop, resultGroup, orientation, anchor, newTextFrame);
    //Create the ATE range
    ATE::TextRangeRef textRangeRef;
    sAITextFrame->GetATETextRange(*newTextFrame, &textRangeRef);
    ATE::ITextRange textRange(textRangeRef);
    textRange.Remove();
    
    return textRange;
}

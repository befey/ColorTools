//
//  BtTransformArt.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/2/16.
//
//

#include "BtTransformArt.hpp"
#include "ArtTree.h"
#include "BtArtHandle.hpp"
#include <cmath>

void RotateArt(AIArtHandle art, AIRealPoint anchor, const AIReal angle)
{
    AIRealMatrix transformMatrix;
    
    //Move to origin
    sAIRealMath->AIRealMatrixSetTranslate(&transformMatrix, -anchor.h, -anchor.v);
    //Rotate
    sAIRealMath->AIRealMatrixConcatRotate(&transformMatrix, sAIRealMath->DegreeToRadian(angle));
    //Move back to original position
    sAIRealMath->AIRealMatrixConcatTranslate(&transformMatrix, anchor.h, anchor.v);
    
    sAITransformArt->TransformArt(art, &transformMatrix, 1, kTransformObjects);
}

void TransformArt(AIArtHandle art, AIReal h, AIReal v)
{
    AIRealMatrix transformMatrix;
    sAIRealMath->AIRealMatrixSetTranslate(&transformMatrix, h, v);
    sAITransformArt->TransformArt(art, &transformMatrix, 1, kTransformObjects);
}

void ResizeRectangle(AIArtHandle path, AIRealRect newRect, Direction hDirection, Direction vDirection)
{
    AIRealMatrix transformMatrix;
    AIRealRect currBounds = {0,0,0,0};
    sAIArt->GetArtBounds(path, &currBounds);
    
    //Move to origin
    AIReal dx, dy;
    if (hDirection == Left) dx = currBounds.left;
    else dx = currBounds.right;
    if (vDirection == Top) dy = currBounds.top;
    else dy = currBounds.bottom;
    
    sAIRealMath->AIRealMatrixSetTranslate(&transformMatrix, -dx, -dy);
    
    AIReal h = std::abs(newRect.right - newRect.left) / std::abs(currBounds.right - currBounds.left);
    AIReal v = std::abs(newRect.bottom - newRect.top) / std::abs(currBounds.bottom - currBounds.top);
    
    //Apply scale
    sAIRealMath->AIRealMatrixConcatScale(&transformMatrix, h, v);
    
    //Move back to original position
    sAIRealMath->AIRealMatrixConcatTranslate(&transformMatrix, dx, dy);
    
    sAITransformArt->TransformArt(path, &transformMatrix, 1, kTransformObjects);
}

void MoveArtOutsideBounds(AIArtHandle art, AIRealRect bounds, Direction dir, AIReal offset)
{
    AIRealRect artRect = BtArtHandle(art).Bounds();
    
    AIReal hMove = 0, vMove = 0;
    
    switch (dir)
    {
        case Direction::Top:
            vMove = bounds.top - artRect.bottom - offset;
            break;
        case Direction::Bottom:
            hMove = bounds.bottom - artRect.top + offset;
            break;
        case Direction::Left:
            hMove = bounds.left - artRect.right - offset;
            break;
        case Direction::Right:
            hMove = bounds.right - artRect.left + offset;
            break;
    }
    
    TransformArt(art, hMove, vMove);
}

AIRealRect FitTextFrameToContents(AIArtHandle textFrameArt)
{
    AIRealRect rect = {0,0,0,0};
    short type;
    sAIArt->GetArtType(textFrameArt, &type);
    
    if (type == kTextFrameArt)
    {
        AITextFrameType textFrameType;
        sAITextFrame->GetType(textFrameArt, &textFrameType);
        if (textFrameType == kInPathTextType)
        {
            TextRangeRef textRange;
            sAITextFrame->GetATETextRange(textFrameArt, &textRange);
            sAIATETextUtil->GetBoundsFromTextRange(textRange, &rect);
            AIArtHandle path = nullptr;
            sAITextFrame->GetPathObject(textFrameArt, &path);
            bool isAssigned;
            ATE::ParagraphJustification justification = ATE::ITextRange(textRange).GetUniqueLocalParaFeatures().GetJustification(&isAssigned);
            
            if (justification == ATE::kRightJustify || justification == ATE::kFullJustifyLastLineRight)
            {
                ResizeRectangle(path, rect, Direction::Right, Direction::Top);
            }
            else
            {
                ResizeRectangle(path, rect, Direction::Left, Direction::Top);
            }
        }
    }
    
    return rect;
}

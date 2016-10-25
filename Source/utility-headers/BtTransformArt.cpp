//
//  BtTransformArt.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/2/16.
//
//

#include "BtTransformArt.hpp"
#include "ArtTree.h"

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

void MoveArtOffArtboard(AIArtHandle art, Direction dir, AIReal offset)
{
    AIRealRect abRect = GetArtboardBounds(GetArtboardOfArt(art));
    AIRealRect artRect = GetBoundsOfArt(art);
    
    AIReal hMove = 0, vMove = 0;
    
    switch (dir)
    {
        case Direction::Top:
            vMove = abRect.top - artRect.bottom - offset;
            break;
        case Direction::Bottom:
            hMove = abRect.bottom - artRect.top + offset;
            break;
        case Direction::Left:
            hMove = abRect.left - artRect.right - offset;
            break;
        case Direction::Right:
            hMove = abRect.right - artRect.left + offset;
            break;
    }
    
    TransformArt(art, hMove, vMove);
}

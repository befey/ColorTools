//
//  BtTransformArt.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/2/16.
//
//

#include "BtTransformArt.hpp"

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
//
//  BtTransformArt.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/2/16.
//
//

#include "BtTransformArt.hpp"

void RotateArt(AIArtHandle art, const AIReal angle)
{
    AIRealMatrix transformMatrix;
    sAIRealMath->AIRealMatrixSetRotate(&transformMatrix, angle);
    
    sAITransformArt->TransformArt(art, &transformMatrix, 1, kTransformObjects);
}
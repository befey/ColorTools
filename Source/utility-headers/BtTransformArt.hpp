//
//  BtTransformArt.hpp
//  SafeguardTools
//
//  Created by T431962 on 8/2/16.
//
//

#ifndef BtTransformArt_hpp
#define BtTransformArt_hpp

#include "AITransformArt.h"
#include "AIRealMath.h"

extern AITransformArtSuite* sAITransformArt;
extern AIRealMathSuite* sAIRealMath;

enum Direction
{
    Top,
    Right,
    Bottom,
    Left
};

void RotateArt(AIArtHandle art, AIRealPoint anchor, const AIReal angle);
void TransformArt(AIArtHandle art, AIReal h, AIReal v);
void MoveArtOffArtboard(AIArtHandle art, Direction dir, AIReal offset);

#endif /* BtTransformArt_hpp */

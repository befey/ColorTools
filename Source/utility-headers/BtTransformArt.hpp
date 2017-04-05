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
#include "AIArt.h"
#include "AITextFrame.h"
#include "AIATETextUtil.h"

extern AITransformArtSuite* sAITransformArt;
extern AIRealMathSuite* sAIRealMath;
extern AIArtSuite* sAIArt;
extern AITextFrameSuite* sAITextFrame;
extern AIATETextUtilSuite* sAIATETextUtil;

enum Direction
{
    Top,
    Right,
    Bottom,
    Left
};

void RotateArt(AIArtHandle art, AIRealPoint anchor, const AIReal angle);
void TransformArt(AIArtHandle art, AIReal h, AIReal v);
void ResizeRectangle(AIArtHandle path, AIRealRect newRect, Direction hDirection, Direction vDirection);
void MoveArtOutsideBounds(AIArtHandle art, AIRealRect bounds, Direction dir, AIReal offset);
AIRealRect FitTextFrameToContents(AIArtHandle textFrameArt);

#endif /* BtTransformArt_hpp */

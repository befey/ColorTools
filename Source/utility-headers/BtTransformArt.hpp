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

void RotateArt(AIArtHandle art, AIRealPoint anchor, const AIReal angle);

#endif /* BtTransformArt_hpp */

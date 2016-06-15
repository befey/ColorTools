/*
 *  FixFreehandType.h
 *  TextTools
 *
 *  Created by Comp Mac 8a on 11/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "AIDocumentList.h"
#include "AITransformArt.h"

extern AIUndoSuite *sAIUndo;
extern AIDocumentListSuite *sAIDocumentList;
extern AIArtSetSuite *sAIArtSet;
extern AIRealMathSuite *sAIRealMath;
extern AITransformArtSuite *sAITransformArt;

bool FixFreehandType();

//Text Frame processing callbacks
bool FixTextAttributes(ATE::ITextRange theRange, ASReal firstRunBaselineShift);
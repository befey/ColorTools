/*
 *  ArtTree.h
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 11/13/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef ARTTREE_H
#define ARTTREE_H

#include "AILayer.h"
#include "AIGroup.h"
#include "AIPath.h"
#include "AIArtboard.h"
#include <functional>
#include <vector>

extern AIArtSetSuite* sAIArtSet;
extern AIArtSuite* sAIArt;
extern AILayerSuite* sAILayer;
extern AIPlacedSuite* sAIPlaced;
extern AIGroupSuite* sAIGroup;
extern AIPathSuite* sAIPath;
extern AIRealMathSuite* sAIRealMath;
extern AIArtboardSuite* sAIArtboard;


bool ProcessArtSet(const AIArtSet artSet, std::function<void(AIArtHandle)> callback);

long CreateArtSetOfPrintingObjectsWithinRect(AIArtSet const targetSet, AIRealRect rect);

void PutArtInGroup(AIArtHandle theArt, AIArtHandle theGroup); //Moves the art to the group, preserving editability flags

bool AllLinkedFilesValid(); //Checks whether the paths for linked files are valid, if not, returns false

AIArtHandle FindTopLevelParent(AIArtHandle currArtHandle); //Finds the top parent object of the passed handle. Returns passed handle if no parents.

bool IsArtInArtSet(AIArtSet theSet, AIArtHandle theObject);

void GetBoundsOfSelectionFromRoot(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect* bounds, bool* boundsValid);
//Find the bounds of the tree of objects under root

void GetBoundsOfClipGroup(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect* bounds, bool* boundsValid);
//Traverses the tree defined by "root" and returns the bounds of the clipping masks contained

AIArtHandle GetGroupArtOfFirstEditableLayer();

int GetArtboardCount();
int GetArtboardOfArt(AIArtHandle artHandle);
AIRealRect GetArtboardBounds(int index);
AIRealPoint GetCenterOfArt(AIArtHandle art);
AIRealRect GetBoundsOfArt(AIArtHandle art);

#endif //ARTTREE_H

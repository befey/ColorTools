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
#include "AIArt.h"
#include "AIPath.h"
#include "AIPathStyle.h"
#include "IDrawer.h"
#include "AIArtboard.h"
#include "AIGeometry.h"
#include <functional>
#include <map>

extern AIArtSetSuite* sAIArtSet;
extern AILayerSuite* sAILayer;
extern AIPlacedSuite* sAIPlaced;
extern AIGroupSuite* sAIGroup;
extern AIArtSuite* sAIArt;
extern AIPathSuite* sAIPath;
extern AIPathStyleSuite* sAIPathStyle;
extern AIRealMathSuite* sAIRealMath;
extern AIArtboardSuite* sAIArtboard;
extern AIGeometrySuite* sAIGeometry;


bool ProcessArtSet(const AIArtSet artSet, std::function<void(AIArtHandle)> callback);

long CreateArtSetOfPrintingObjectsWithinRect(AIArtSet const targetSet, AIRealRect rect);

bool AllLinkedFilesValid(); //Checks whether the paths for linked files are valid, if not, returns false

AIArtHandle FindTopLevelParent(AIArtHandle currArtHandle); //Finds the top parent object of the passed handle. Returns passed handle if no parents.

void GetBoundsOfSelectionFromRoot(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect* bounds, bool* boundsValid);
//Find the bounds of the tree of objects under root

void GetBoundsOfClipGroup(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect* bounds, bool* boundsValid);
//Traverses the tree defined by "root" and returns the bounds of the clipping masks contained

AIArtHandle GetGroupArtOfFirstEditableLayer();

void SelectArt(AIArtHandle artHandle);

int GetArtboardCount();
int GetArtboardOfArt(AIArtHandle artHandle);
AIRealRect GetArtboardBounds(int index);
AIRealPoint GetCenterOfArt(AIArtHandle art);
AIRealRect GetBoundsOfArt(AIArtHandle art);
AIRealPoint GetCenterOfRect(AIRealRect rect);
map<int,AIArtHandle> GetArtboardOfArts(vector<AIArtHandle> pluginArts);

AIArtHandle DrawRectangle(AIRealRect rect, AIArtHandle prep);

int GetExpansionAmountToContainRect(AIRealRect rect1, AIRealRect rect2);

bool operator==(const AIRealRect& lhs, const AIRealRect& rhs);
inline bool operator!=(const AIRealRect& lhs, const AIRealRect& rhs) { return !operator==(lhs,rhs); };

#endif //ARTTREE_H

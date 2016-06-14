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

extern AIArtSetSuite* sAIArtSet;
extern AIArtSuite* sAIArt;
extern AILayerSuite* sAILayer;
extern AIPlacedSuite* sAIPlaced;
extern AIGroupSuite* sAIGroup;
extern AIPathSuite* sAIPath;
extern AIRealMathSuite* sAIRealMath;

//AIArtHandle GetArtObjectByName(ai::UnicodeString targetName, Corner CORNER, Node* const startNode = NULL, Node* const currNode = NULL);
 //Returns an art object by finding the first occurence of the name in the layer list
 //If the last two arguments are supplied, returns the object with the name that is closest to the node
 
long CreateArtSetFromLayer(ai::UnicodeString layerName, AIArtSet const targetSet); //Adds to the art set all the objects on a layer, returns the number of objects in the set

long CreateArtSetOfPrintingObjectsWithinRect(AIArtSet const targetSet, AIRealRect rect);

void MoveArtToTopOfLayer(AIArtHandle currArtHandle); //Checks whether the art is at the top level of its layer, moves it there if not

//Moves the art to the top of the passed layer, if the layer can't be found, moves to the top of current layer
void PutArtAtTopOfLayer(AIArtHandle art, string layerName);

void PutArtInGroup(AIArtHandle theArt, AIArtHandle theGroup); //Moves the art to the group, preserving editability flags

bool AllLinkedFilesValid(); //Checks whether the paths for linked files are valid, if not, returns false

AIArtHandle FindTopLevelParent(AIArtHandle currArtHandle); //Finds the top parent object of the passed handle. Returns passed handle if no parents.

bool IsArtInArtSet(AIArtSet theSet, AIArtHandle theObject);

void GetBoundsOfSelectionFromRoot(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect* bounds, bool* boundsValid);
//Find the bounds of the tree of objects under root

void GetBoundsOfClipGroup(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect* bounds, bool* boundsValid);
//Traverses the tree defined by "root" and returns the bounds of the clipping masks contained

#endif //ARTTREE_H
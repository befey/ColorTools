/*
 *  ArtTree.cpp
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 11/13/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ArtTree.h"


long CreateArtSetOfPrintingObjectsWithinRect(AIArtSet const targetSet, AIRealRect rect)
{
    int count;
    size_t c;
    sAILayer->CountLayers(&count);
    AILayerHandle layer;
    
    for (int i = 0; i < count; i++) {
        sAILayer->GetNthLayer(i, &layer);
        AIBoolean printed;
        sAILayer->GetLayerPrinted(layer, &printed);
        if (printed)
        {
            AIArtSet layerArtSet;
            sAIArtSet->NewArtSet(&layerArtSet);
            sAIArtSet->LayerArtSet(layer, layerArtSet);
            sAIArtSet->CountArtSet(layerArtSet, &c);
            
            for (int j = 0; j < c; j++)
            {
                AIArtHandle currArtHandle;
                sAIArtSet->IndexArtSet(layerArtSet, j, &currArtHandle);
                AIRealRect artRect;
                sAIArt->GetArtBounds(currArtHandle, &artRect);
                int attr;
                sAIArt->GetArtUserAttr(currArtHandle, kArtHidden, &attr);
                short type;
                sAIArt->GetArtType(currArtHandle, &type);
                
                if (sAIRealMath->AIRealRectOverlap(&artRect, &rect) && !(attr & kArtHidden) && type != kGroupArt)
                {
                    sAIArtSet->AddArtToArtSet(targetSet, currArtHandle);
                }
            }
           
            sAIArtSet->DisposeArtSet(&layerArtSet);
        }
    }
    
    sAIArtSet->CountArtSet(targetSet, &c);
    return c;
}

void PutArtInGroup(AIArtHandle currArtHandle, AIArtHandle theGroup) {
	AILayerHandle layer = NULL; sAIArt->GetLayerOfArt(currArtHandle, &layer);
	if (!layer) {
		return;
	}
	AIArtHandle layerGroup; sAIArt->GetFirstArtOfLayer(layer, &layerGroup);

	int eflag = 0; int vflag = 0;
	ASBoolean editable = FALSE;
	ASBoolean visible = FALSE;
	
	int attr = 0;
	
	//Check if the layer is editable
	sAILayer->GetLayerEditable(layer, &editable);
	sAILayer->GetLayerVisible(layer, &visible);
	
	//Set the layer editable and visible, set flags so we can set it back the way it was
	if (!editable) { sAILayer->SetLayerEditable(layer, TRUE); eflag = 1; }
	if (!visible) { sAILayer->SetLayerVisible(layer, TRUE); vflag = 1; }
	
	//Check if the art itself is editable
	sAIArt->GetArtUserAttr(currArtHandle, kArtLocked | kArtHidden, &attr);
	if ((attr & kArtLocked) || (attr & kArtHidden)) {
		sAIArt->SetArtUserAttr(currArtHandle, kArtLocked | kArtHidden, 0);
	}
	
	//Move it out of the group
	sAIArt->ReorderArt(currArtHandle, kPlaceInsideOnTop, theGroup);
	
	//Set the layer and art attributes back the way they were
	if(eflag) { sAILayer->SetLayerEditable(layer, FALSE); }
	if(vflag) { sAILayer->SetLayerVisible(layer, FALSE); }
	sAIArt->SetArtUserAttr(currArtHandle, kArtLocked | kArtHidden, attr);
	
	return;
}

bool AllLinkedFilesValid() {
	size_t count = 0;
	ai::UnicodeString currArtName;
	AIArtHandle currArtObj = NULL;

	AIArtSet artSet; sAIArtSet->NewArtSet(&artSet);
	
	AIArtSpec specs[] = { { kPlacedArt , 0 , 0 } };

	sAIArtSet->MatchingArtSet( specs , 1, artSet );
	
	sAIArtSet->CountArtSet(artSet, &count);
	
	for(int i=0; i < count ; i++) {
		sAIArtSet->IndexArtSet(artSet, i, &currArtObj);
		if(currArtObj) {
			ai::FilePath path;
			sAIPlaced->GetPlacedFileSpecification(currArtObj, path);
			if (!path.Exists(TRUE)) {
				sAIArtSet->DisposeArtSet(&artSet); artSet = NULL;
				return FALSE;
			}
		}
	}
	
	sAIArtSet->DisposeArtSet(&artSet); artSet = NULL;
	return TRUE;
}

AIArtHandle FindTopLevelParent(AIArtHandle currArtHandle) {
	AIArtHandle currParent = NULL;
	AIArtHandle parent = NULL;
	parent = currParent = currArtHandle;
	
	//If we have an object that is part of a group, we need to find the bounds of the top level parent group and use those for how much to move it
	while (currParent) {
		sAIArt->GetArtParent(parent, &currParent);
		if (currParent) {
			ASBoolean isLayer = FALSE;
			sAIArt->IsArtLayerGroup(currParent, &isLayer);
			if (isLayer) {
				sAIArt->GetArtParent(currParent, &currParent);
			} else {
				parent = currParent;
			}
		}
	}
	return parent;
}


bool IsArtInArtSet(AIArtSet artSet, AIArtHandle theObject)
{
	size_t count;		sAIArtSet->CountArtSet( artSet, &count );
	for ( int i=0 ; i < count ; i++ ) {
		AIArtHandle currArtHandle = NULL;
		sAIArtSet->IndexArtSet( artSet, i, &currArtHandle );
		if (currArtHandle == theObject) {
			return TRUE;
		}
	}
	return FALSE;
}


void GetBoundsOfSelectionFromRoot(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect* bounds, bool* boundsValid) {
	
	AIArtHandle child, sibling;
	child = sibling = NULL;
	
	bool boundsSet = FALSE;
	
/*	//Check if this object is in a clipping mask and store the bounds of the mask so we don't get bounds of stuff that's clipped
	AIBoolean clipped = FALSE;
	sAIGroup->GetGroupClipped(currArtHandle, &clipped);
	
	AIArtHandle parent = NULL;
//	sAIArt->GetArtParent(currArtHandle, &parent);
//	while (parent && !clipBoundsSet) {
		AIArtHandle tempHandle = parent;
		
		long attr = 0;
		sAIArt->GetArtUserAttr(currArtHandle, kArtIsClipMask, &attr);
		if (attr & kArtIsClipMask) {
			sAIArt->GetArtTransformBounds(currArtHandle, NULL, kVisibleBounds | kExcludeHiddenObjectBounds | kExcludeGuideBounds, &clipBounds);
			clipBoundsSet = TRUE;
		}
		sAIArt->GetArtParent(tempHandle, &parent);
//	}
*/	
	//TODO: FIX CLIPPING PATH
	// If Fully Selected, start going through children and siblings, stop drilling down when a clipping group is encountered
	// This way, we'll only get the bounds of stuff that isn't inside a clipping mask.
//	long attr = 0;
//	AIBoolean isClipMask = FALSE;
//	sAIArt->GetArtUserAttr(currArtHandle, kArtIsClipMask, &attr);
//	if (attr & kArtIsClipMask) { isClipMask = TRUE; }

	AIBoolean clipped = FALSE;
	sAIGroup->GetGroupClipped(currArtHandle, &clipped);
	
	short artType = 0;
	sAIArt->GetArtType(currArtHandle, &artType);
	if ((artType == kGroupArt) || (artType == kCompoundPathArt)) {
	
		AIRealRect tempBounds;
		bool tempBoundsValid = FALSE;
		
		if (clipped) {
			GetBoundsOfClipGroup(currArtHandle, currArtHandle, &tempBounds, &tempBoundsValid);
		} else {
			sAIArt->GetArtFirstChild(currArtHandle, &child);
			if (child) {
				GetBoundsOfSelectionFromRoot(root, child, &tempBounds, &tempBoundsValid);
			}
		}
		if (tempBoundsValid) {
			//Check if bounds has contents already
			if (!boundsSet) {
				//If we got valid bounds, store these ones
				sAIRealMath->AIRealRectSet(bounds, tempBounds.left, tempBounds.top, tempBounds.right, tempBounds.bottom);
				*boundsValid = TRUE;
				boundsSet = TRUE;
			} else {
				//If we have bounds, Union these bounds with them
				//Set existingBounds = bounds
				AIRealRect existingBounds;
				sAIRealMath->AIRealRectSet(&existingBounds, bounds->left, bounds->top, bounds->right, bounds->bottom);
				
				sAIRealMath->AIRealRectUnion(&existingBounds, &tempBounds, bounds);
				*boundsValid = TRUE;
				boundsSet = TRUE;
			}
		}
	}
	else if (artType == kPathArt){
		//RETURN BOUNDS OF SELECTED SEGMENTS
		short count;
		sAIPath->GetPathSegmentCount(currArtHandle, &count);
		AIPathSegment* segments = NULL;
		segments = new AIPathSegment[count];
		
		AIRealRect segBounds;
		bool segBoundsSetFlag = FALSE;
		
		sAIPath->GetPathSegments(currArtHandle, 0, count, segments);
		
		for (int i=0 ; i<count ; i++) {
			short selected = 0;
			sAIPath->GetPathSegmentSelected(currArtHandle, i, &selected);
			
			if (selected == kSegmentPointSelected) {
				if (!segBoundsSetFlag) {
					//If we don't have bounds yet, store these ones
					sAIRealMath->AIRealRectSet(&segBounds, segments[i].p.h, segments[i].p.v, segments[i].p.h, segments[i].p.v);
					
					sAIRealMath->AIRealRectSet(bounds, segBounds.left, segBounds.top, segBounds.right, segBounds.bottom);
					segBoundsSetFlag = TRUE;
					boundsSet = TRUE;
				} else {
					//If we have segBounds, Union these bounds with them
					AIRealRect tempBounds;
					sAIRealMath->AIRealRectSet(&tempBounds, segBounds.left, segBounds.top, segBounds.right, segBounds.bottom);
					sAIRealMath->AIRealRectSet(&segBounds, segments[i].p.h, segments[i].p.v, segments[i].p.h, segments[i].p.v);
					
					sAIRealMath->AIRealRectUnion(&segBounds, &tempBounds, bounds);
					boundsSet = TRUE;
				}
			}
		}
		delete [] segments;
		if (boundsSet) {
			*boundsValid = TRUE;
		}
	} else {
		//ANY OTHER ART TYPES
		sAIArt->GetArtTransformBounds(currArtHandle, NULL, kVisibleBounds | kExcludeHiddenObjectBounds | kExcludeGuideBounds, bounds);
		
		*boundsValid = TRUE;
		boundsSet = TRUE;			
	}

	//CHECK CURR != ROOT
	if (currArtHandle != root) {
		//GET BOUNDS OF SIBLING
		sAIArt->GetArtSibling(currArtHandle, &sibling);
		if (sibling) {
			AIRealRect tempBounds;
			bool tempBoundsValid = FALSE;
			GetBoundsOfSelectionFromRoot(root, sibling, &tempBounds, &tempBoundsValid);
			if (tempBoundsValid) {

				//Check if bounds has contents already
				if (!boundsSet) {
					//If we got valid bounds, store these ones
					sAIRealMath->AIRealRectSet(bounds, tempBounds.left, tempBounds.top, tempBounds.right, tempBounds.bottom);
					boundsSet = TRUE;
					*boundsValid = TRUE;
				} else {
					//If we have bounds, Union these bounds with them
					//Set existingBounds = bounds
					AIRealRect existingBounds;
					sAIRealMath->AIRealRectSet(&existingBounds, bounds->left, bounds->top, bounds->right, bounds->bottom);
					
					sAIRealMath->AIRealRectUnion(&existingBounds, &tempBounds, bounds);
					boundsSet = TRUE;
					*boundsValid = TRUE;
				}
			}
		}
	}
}


void GetBoundsOfClipGroup(AIArtHandle root, AIArtHandle currArtHandle, AIRealRect* bounds, bool* boundsValid) {
	AIArtHandle child, sibling;
	child = sibling = NULL;
	
	bool boundsSet = FALSE;
	
	int attr = 0;

	sAIArt->GetArtUserAttr(currArtHandle, kArtIsClipMask, &attr);
	if (attr & kArtIsClipMask) {
		sAIArt->GetArtTransformBounds(currArtHandle, NULL, kVisibleBounds | kExcludeHiddenObjectBounds | kExcludeGuideBounds, bounds);
		
		*boundsValid = TRUE;
		boundsSet = TRUE;	
	} else {
		short artType = 0;
		sAIArt->GetArtType(currArtHandle, &artType);
		
		if ((artType == kGroupArt) || (artType == kCompoundPathArt)) {
			sAIArt->GetArtFirstChild(currArtHandle, &child);
			if (child) {
				AIRealRect tempBounds;
				bool tempBoundsValid = FALSE;
				GetBoundsOfClipGroup(root, child, &tempBounds, &tempBoundsValid);
				
				if (tempBoundsValid) {
					//Check if bounds has contents already
					if (!boundsSet) {
						//If we got valid bounds, store these ones
						sAIRealMath->AIRealRectSet(bounds, tempBounds.left, tempBounds.top, tempBounds.right, tempBounds.bottom);
						boundsSet = TRUE;
						*boundsValid = TRUE;
					} else {
						//If we have bounds, Union these bounds with them
						//Set existingBounds = bounds
						AIRealRect existingBounds;
						sAIRealMath->AIRealRectSet(&existingBounds, bounds->left, bounds->top, bounds->right, bounds->bottom);
						
						sAIRealMath->AIRealRectUnion(&existingBounds, &tempBounds, bounds);
						boundsSet = TRUE;
						*boundsValid = TRUE;
					}
				}
			}			
		}
	}
	
	//CHECK CURR != ROOT
	if (currArtHandle != root) {
		//GET BOUNDS OF SIBLING
		sAIArt->GetArtSibling(currArtHandle, &sibling);
		if (sibling) {
			AIRealRect tempBounds;
			bool tempBoundsValid = FALSE;
			GetBoundsOfClipGroup(root, sibling, &tempBounds, &tempBoundsValid);
			
			if (tempBoundsValid) {
				//Check if bounds has contents already
				if (!boundsSet) {
					//If we got valid bounds, store these ones
					sAIRealMath->AIRealRectSet(bounds, tempBounds.left, tempBounds.top, tempBounds.right, tempBounds.bottom);
					boundsSet = TRUE;
					*boundsValid = TRUE;
				} else {
					//If we have bounds, Union these bounds with them
					//Set existingBounds = bounds
					AIRealRect existingBounds;
					sAIRealMath->AIRealRectSet(&existingBounds, bounds->left, bounds->top, bounds->right, bounds->bottom);
					
					sAIRealMath->AIRealRectUnion(&existingBounds, &tempBounds, bounds);
					boundsSet = TRUE;
					*boundsValid = TRUE;
				}
			}
		}
	}
}

bool ProcessArtSet(const AIArtSet artSet, std::function<void(AIArtHandle)> callback)
{
    size_t count;
    sAIArtSet->CountArtSet(artSet, &count);
    for (int i = 0; i < count; i++)
    {
        AIArtHandle art;
        sAIArtSet->IndexArtSet(artSet, i, &art);
        callback(art);
    }
    return true;
}
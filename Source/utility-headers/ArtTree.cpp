/*
 *  ArtTree.cpp
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 11/13/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ArtTree.h"
#include "BtLayer.hpp"
#include "BtArtHandle.hpp"
#include "GetIllustratorErrorCode.h"
#include <map>

long CreateArtSetOfPrintingObjectsWithinRect(AIArtSet const targetSet, AIRealRect rect)
{
    int count;
    size_t c;
    sAILayer->CountLayers(&count);
    BtLayer layer;
    
    for (int i = 0; i < count; i++) {
        sAILayer->GetNthLayer(i, layer);
        if (layer.Printed())
        {
            AIArtSet layerArtSet;
            sAIArtSet->NewArtSet(&layerArtSet);
            sAIArtSet->LayerArtSet(layer, layerArtSet);
            sAIArtSet->CountArtSet(layerArtSet, &c);
            
            for (int j = 0; j < c; j++)
            {
                BtArtHandle currArtHandle;
                sAIArtSet->IndexArtSet(layerArtSet, j, currArtHandle);
                if (currArtHandle.ArtType() == kGroupArt)
                {
                    continue;
                }
                
                if (currArtHandle.OverlapsRect(rect) && !currArtHandle.Hidden())
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

bool AllLinkedFilesValid()
{
	size_t count = 0;
	ai::UnicodeString currArtName;
	AIArtHandle currArtObj = NULL;

	AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
	
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
	
	sAIArtSet->DisposeArtSet(&artSet);
	return TRUE;
}

AIArtHandle FindTopLevelParent(AIArtHandle currArtHandle)
{
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

AIArtHandle GetGroupArtOfFirstEditableLayer()
{
    ai::int32 count;
    sAILayer->CountLayers(&count);
    
    for (int i = 0; i < count; i++)
    {
        AILayerHandle layer;
        sAILayer->GetNthLayer(i, &layer);
        BtLayer btLayer(layer);
        if (btLayer.Editable() && btLayer.Visible())
        {
            return btLayer.GetLayerGroupArt();
        }
    }
    
    return NULL;
}

int GetArtboardCount()
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardID count;
    sAIArtboard->GetCount(abList, count);
    return count;
}

int GetArtboardOfArt(AIArtHandle artHandle)
{
    vector<AIArtHandle> v {artHandle};
    return GetArtboardOfArts(v).begin()->first;
}

AIRealRect GetArtboardBounds(int index)
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties props;
    sAIArtboard->GetArtboardProperties(abList, index, props);
    AIRealRect rect;
    props.GetPosition(rect);
    return rect;
}

AIRealPoint GetCenterOfRect(AIRealRect rect)
{
    AIRealPoint center;
    AIRealPoint a = {.h = rect.left, .v = rect.top};
    AIRealPoint b = {.h = rect.right, .v = rect.bottom};
    sAIRealMath->AIRealPointInterpolate(&a, &b, .5, &center);
    return center;
}

map<int,AIArtHandle> GetArtboardOfArts(vector<AIArtHandle> arts)
{
    vector<tuple<int,AIArtHandle,AIReal>> d;
    
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardID count;
    sAIArtboard->GetCount(abList, count);
    
    //Calculate the distance of each art from each artboard
    for ( int i = 0; i < count; i++ )
    {
        AIRealPoint abCenter = GetCenterOfRect(GetArtboardBounds(i));
        
        for ( auto ah : arts )
        {
            AIRealPoint artCenter = BtArtHandle(ah).Center();
            AIReal distance = sAIRealMath->AIRealPointLength(&abCenter, &artCenter);
            
            d.push_back( {i, ah, distance} );
        }
    }
    
    //Sort by distance
    std::sort(d.begin(), d.end(),
              [](auto &left, auto &right)
              {
                  return std::get<2>(left) < std::get<2>(right);
              });
    
    //Starting with the art that is closest to an artboard, remove all arts which are not closest to that artboard
    for(auto item = d.begin(); item != d.end(); item++)
    {
        bool found1 = false;
        d.erase(
                std::remove_if(item, d.end(),
                               [item, &found1](tuple<int,AIArtHandle,AIReal> curr)
                               {
                                   if (std::get<1>(*item) == std::get<1>(curr) || std::get<0>(*item) == std::get<0>(curr))
                                   {
                                       if (!found1)
                                       {
                                           found1 = true;
                                           return false;
                                       }
                                       else
                                       {
                                           return true;
                                       }
                                   }
                                   return false;
                               }
                               ),
                d.end()
                );
    }
    
    //Sort by artboard index
    std::sort(d.begin(), d.end(),
              [](auto &left, auto &right)
              {
                  return std::get<0>(left) < std::get<0>(right);
              });
    
    //Create the vector of just ArtHandles
    map<int,AIArtHandle> result;
    for ( auto m : d )
    {
        result.insert( std::pair<int,AIArtHandle>{std::get<0>(m), std::get<1>(m)} );
    }
    return result;
}

AIArtHandle DrawRectangle(AIRealRect rect, AIArtHandle prep)
{
    AIArtHandle pathArt;
    sAIArt->NewArt(kPathArt, kPlaceInsideOnTop, prep, &pathArt);
    
    AIRealPoint topLeft = {.h = rect.left, .v = rect.top};
    AIRealPoint topRight = {.h = rect.right, .v = rect.top};
    AIRealPoint bottomLeft = {.h = rect.left, .v = rect.bottom};
    AIRealPoint bottomRight = {.h = rect.right, .v = rect.bottom};
    AIPathSegment segments[4];
    segments[0] = { .corner = true, .p = topLeft, .in = topLeft, .out = topLeft };
    segments[1] = { .corner = true, .p = topRight, .in = topRight, .out = topRight };
    segments[2] = { .corner = true, .p = bottomRight, .in = bottomRight, .out = bottomRight };
    segments[3] = { .corner = true, .p = bottomLeft, .in = bottomLeft, .out = bottomLeft };
    
    sAIPath->SetPathSegments(pathArt, 0, 4, segments);
    sAIPath->SetPathClosed(pathArt, true);
    
    return pathArt;
}

int GetExpansionAmountToContainRect(AIRealRect rect1, AIRealRect rect2)
{
    int diff = 0;
    
    while ( !sAIRealMath->AIRealRectInAIRealRect(&rect2, &rect1) )
    {
        sAIRealMath->AIRealRectInset(&rect1, -1, -1);
        diff++;
    }
    
    return diff;
}

bool operator==(const AIRealRect& lhs, const AIRealRect& rhs)
{
    return (lhs.bottom == rhs.bottom) && (lhs.left == rhs.left) && (lhs.right == rhs.right) && (lhs.top == rhs.top);
}

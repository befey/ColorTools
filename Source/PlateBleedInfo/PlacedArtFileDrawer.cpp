//
//  PlacedArtFileDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 11/1/16.
//
//

#include "PlacedArtFileDrawer.hpp"
#include "SafeguardFileConstants.h"
#include "GetIllustratorErrorCode.h"
#include "ArtTree.h"
#include "BtTransformArt.hpp"

using SafeguardFile::PlacedArtFileDrawer;

AIArtHandle PlacedArtFileDrawer::DoDraw(AIArtHandle resultGroup) const
{
    AIArtHandle placedArtHandle = NULL;
    sAIArt->NewArt(kPlacedArt, kPlaceInsideOnTop, resultGroup, &placedArtHandle);
    
    AIRealRect bbox = bounds;
    sAIPlaced->SetPlacedBoundingBox(placedArtHandle, &bbox);
    sAIPlaced->SetPlaceOptions(placedArtHandle, kReconform, kBotMid, TRUE);
    
    //Harden matrix and invert
    AIRealMatrix matrix;
    sAIPlaced->GetPlacedMatrix(placedArtHandle, &matrix);
    sAIHardSoft->AIRealMatrixRealHard(&matrix);
    sAIRealMath->AIRealMatrixConcatScale(&matrix, 1, -1);
    sAIPlaced->SetPlacedMatrix(placedArtHandle, &matrix);
    
    sAIPlaced->SetPlacedFileSpecification(placedArtHandle, path);
    
    AIArtSet selectedArtSet;
    sAIArtSet->NewArtSet(&selectedArtSet);
    sAIArtSet->SelectedArtSet(selectedArtSet);
    
    AIArtHandle nativeArtHandle = NULL;
    sAIPlaced->MakePlacedObjectNative(placedArtHandle, &nativeArtHandle, FALSE);
    
    ProcessArtSet(selectedArtSet, SelectArt);
    
    sAIArtSet->DisposeArtSet(&selectedArtSet);
    
    return nativeArtHandle;
}

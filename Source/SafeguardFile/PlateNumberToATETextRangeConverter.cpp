//
//  PlateNumberToATETextRangeConverter.cpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/28/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#include "PlateNumberToATETextRangeConverter.hpp"

void SafeguardFile::PlateNumberToATETextRangeConverter::GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const
{
    AddTextToRange(pn, targetRange);
}

Boolean PlateNumber::HasInnerTicks() const
{
    size_t count = 0;
    AIArtSet artSet = nullptr;
    sAIArtSet->NewArtSet(&artSet);
    
    AIArtHandle currArtHandle = nullptr;
    
    AIArtSpec specs[] = { { kPathArt , 0 , 0 } };
    
    sAIArtSet->MatchingArtSet( specs , 1, artSet );
    
    sAIArtSet->CountArtSet(artSet, &count);
    if(count == 0) {
        return false;
    }
    //Loop through the art set
    for (int i = 0 ; i < count ; i++) {
        
        sAIArtSet->IndexArtSet(artSet, i, &currArtHandle);
        AIBoolean closed;
        sAIPath->GetPathClosed(currArtHandle, &closed);
        if (!closed)
        {
            AIRealRect artBounds; sAIArt->GetArtBounds(currArtHandle, &artBounds);
            ai::ArtboardList artboardList;
            ai::ArtboardProperties props;
            sAIArtboard->GetArtboardList(artboardList);
            sAIArtboard->GetArtboardProperties(artboardList, 0, props);
            AIRealRect pageBounds;
            sAIArtboard->GetPosition(props, pageBounds);
            
            AIBoolean overlap = sAIRealMath->AIRealRectOverlap(&pageBounds, &artBounds);
            AIReal length;
            sAIPath->GetPathLength(currArtHandle, &length, 0);
            if (overlap && sAIRealMath->EqualWithinTol(length, LENGTH_OF_INNER_TICK_PATH, 1))
            {
                if ( ColorEnumerator(currArtHandle).HasRegistrationColor() )
                {
                    return true;
                }
            }
        }
    }
    sAIArtSet->DisposeArtSet(&artSet);
    
    return false;
}

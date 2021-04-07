//
//  ArtObjectFilter.cpp
//  SafeguardTools
//
//  Created by T431962 on 2/9/18.
//
//

#include "ArtObjectFilter.hpp"
#include "ArtTree.h"
#include "BtArtHandle.hpp"

bool ArtObjectColorFilter::Execute() const
{
    size_t count;
    sAIArtSet->CountArtSet(*artSet, &count);
    
    AIArtSet artMatchingColorsSet;
    sAIArtSet->NewArtSet(&artMatchingColorsSet);
    
    for (int i = 0; i < count; i++)
    {
        AIArtHandle art;
        sAIArtSet->IndexArtSet(*artSet, i, &art);
        BtArtHandle btArt(art);
        for ( auto color : colors )
        {
            if ( btArt.FillColor() == color || btArt.StrokeColor() == color )
            {
                sAIArtSet->AddArtToArtSet(artMatchingColorsSet, btArt);
                break;
            }
        }
    }
    
    sAIArtSet->ClearArtSet(*artSet);
    
    sAIArtSet->CountArtSet(artMatchingColorsSet, &count);
    for (int i = 0; i < count; i++)
    {
        AIArtHandle art;
        sAIArtSet->IndexArtSet(*artSet, i, &art);
        sAIArtSet->AddArtToArtSet(*artSet, art);
    }

    return true;

}

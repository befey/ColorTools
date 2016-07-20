//
//  FindReplaceGraphics.cpp
//  SafeguardTools
//
//  Created by T431962 on 5/21/15.
//
//

#include "FindReplaceGraphics.h"
#include "ColorFuncs.h"
#include "ReplaceData.h"

#include "string.h"
#include "stdlib.h"


int FindAndReplace(unique_ptr<ReplaceData> &data)
{
    AIBoolean flag = FALSE;
    int numChanged = 0;
    
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    
    fillArtSet( artSet , data->changein );

    size_t count;
    sAIArtSet->CountArtSet( artSet, &count );
    
    for ( int i=0 ; i < count ; i++ )
    {
        AIArtHandle currArtObj;
        sAIArtSet->IndexArtSet( artSet, i, &currArtObj );
        
        /*********** FIND AND REPLACE COLORS *************/
        if ( data->attribute == ColorToolsUIController::Attribute::Color )
        {
            //do the color replacing
            sAIPathStyle->AdjustObjectAIColors( currArtObj , adjustColor , data.get() , data->controlFlags , &flag );
           
            //increment counter if a switch was made
            if (flag)
            {
                numChanged++;
            }
            flag = FALSE;
        }
        
        /*********** FIND AND REPLACE OVERPRINTING *************/
        if ( data->attribute == ColorToolsUIController::Attribute::Overprint )
        {
            AIBoolean op = FALSE;
            if (data->addremove == ColorToolsUIController::AddRemove::Add)
            {
                op = TRUE;
            }
            AdjustOverprint(currArtObj, data->fromColor, data->changeTints, op, data->applyto, &flag);
            
            if (flag)
            {
                numChanged++;
            }
            flag = FALSE;
        }
    }
    
    //DISPOSE OF THE ART SET
    sAIArtSet->DisposeArtSet(&artSet);
    
    return numChanged;
}

void fillArtSet( AIArtSet &artSet, ColorToolsUIController::ChangeIn changeIn)
{
    AIArtSpec selectedSpecs[] =
    {
        { kPathArt , kArtFullySelected , kArtFullySelected },
        { kCompoundPathArt , kArtFullySelected , kArtFullySelected },
        { kPlacedArt , kArtFullySelected , kArtFullySelected },
        { kRasterArt , kArtFullySelected , kArtFullySelected },
        { kPluginArt , kArtFullySelected , kArtFullySelected },
        { kMeshArt , kArtFullySelected , kArtFullySelected },
        { kTextFrameArt , kArtFullySelected , kArtFullySelected },
        { kSymbolArt , kArtFullySelected , kArtFullySelected },
    };
    AIArtSpec allSpecs[] =
    {
        { kPathArt , 0 , 0 },
        { kCompoundPathArt , 0 , 0 },
        { kPlacedArt , 0 , 0 },
        { kRasterArt , 0 , 0 },
        { kPluginArt , 0 , 0 },
        { kMeshArt , 0 , 0 },
        { kTextFrameArt , 0 , 0 },
        { kSymbolArt , 0 , 0 },
    };
    //SELECTION
    if (changeIn == ColorToolsUIController::ChangeIn::Selection)
    {
        sAIArtSet->MatchingArtSet( selectedSpecs , 8, artSet );
    }
    //PAGE or DOCUMENT for now
    if (changeIn == ColorToolsUIController::ChangeIn::Document)
    {
        sAIArtSet->MatchingArtSet( allSpecs , 8, artSet );
    }
}

void adjustColor(AIColor *color, void* userData, AIErr *result, AIBoolean *altered)
{
    ReplaceData* data = (ReplaceData*)userData;
    
    *altered = FALSE;
    
    AISwatchRef fromSwatchRef = sAISwatchList->GetSwatchByColor(NULL, &data->fromColor);
    AISwatchRef colorSwatchRef = sAISwatchList->GetSwatchByColor(NULL, color);
    
    if (color->kind == kGrayColor ||
        color->kind == kFourColor ||
        color->kind == kCustomColor ||
        color->kind == kThreeColor ||
        color->kind == kNoneColor )
    {
        if ( ColorIsEqual( data->fromColor , *color , TRUE /*ignoreTints*/  ) )
        {
            if ( data->fromColor.c.c.tint == color->c.c.tint )
            {   //IF THE TINTS ARE THE SAME
                if (fromSwatchRef == colorSwatchRef)
                {
                    *color = data->toColor;
                    *altered = TRUE;    //Change to the To color
                }
            }
            else
            {   //IF THE TINTS ARE DIFFERENT
                if (data->changeTints)    //If we are changing the different tints
                {
                    AIColor tempColor = data->toColor;  //Make a new temporary color that is the same as the ToColor,
                    tempColor.c.c.tint = color->c.c.tint;   //except the tint is the same as the object's
                    
                    AISwatchRef toColorTintSwatch = checkSwatchListForColor(tempColor, .01);
                    
                    if ( toColorTintSwatch )
                    { //If the swatch already exists
                        sAISwatchList->GetAIColor(toColorTintSwatch, color);
                        *altered = TRUE;    //Send back the color of the existing swatch
                    }
                    else
                    {
                        //Insert a new Swatch into the main group
                        toColorTintSwatch = sAISwatchList->InsertNthSwatch(NULL, -1);
                        if (toColorTintSwatch)
                        {
                            //Apply the tempColor to the new swatch
                            sAISwatchList->SetAIColor(toColorTintSwatch, &tempColor);
                            *color = tempColor;
                            *altered = TRUE;    //Send back tempColor (ToColor with the current Object's Tint)
                        }
                    }
                }
            }
        }
    }
}
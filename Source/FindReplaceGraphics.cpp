//
//  FindReplaceGraphics.cpp
//  ColorTools
//
//  Created by T431962 on 5/21/15.
//
//

#include "FindReplaceGraphics.h"
#include "ColorFuncs.h"
#include "ReplaceData.h"

#include "string.h"
#include "stdlib.h"


int FindAndReplace(ReplaceData* data) {
    
    AIBoolean flag = FALSE;
    int numChanged = 0;
    
    //CREATE THE ART SET
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    
    //FILL THE ART SET BASED ON Change in Selection or Document
    fillArtSet( artSet , data->changeinSelect );
  
    
    //LOOP THROUGH THE SET AND CHECK THE STROKES AND FILLS FOR THE COLOR WE're CHANGING from

    size_t count;		sAIArtSet->CountArtSet( artSet, &count );
    
    for ( int i=0 ; i < count ; i++ ) {
        AIArtHandle currArtObj;
        sAIArtSet->IndexArtSet( artSet, i, &currArtObj );
      
        
        /*********** FIND AND REPLACE COLORS *************/
        if ( data->attributeSelect == ATTRIBUTE_COLOR ) {
            
            //do the color replacing
            sAIPathStyle->AdjustObjectAIColors( currArtObj , adjustColor , data , data->controlFlags , &flag );
           
            //increment counter if a switch was made
            if (flag) { numChanged++; }
            flag = FALSE;
        }
        
        
        /*********** FIND AND REPLACE OVERPRINTING *************/
        if ( data->attributeSelect == ATTRIBUTE_OVERPRINT ) {
      
  
            AdjustOverprint(currArtObj, *data->fromColor, data->tintsCheckbox, data->addremoveSelect, data->applytoSelect, &flag);
            
            if (flag) { numChanged++; }
            flag = FALSE;
        }
    }
    
    //DISPOSE OF THE ART SET
    sAIArtSet->DisposeArtSet(&artSet);
    
    return numChanged;
}

void fillArtSet( AIArtSet &artSet, int changeIn) {
    
    AIArtSpec selectedSpecs[] = { { kPathArt , kArtFullySelected , kArtFullySelected },
        { kCompoundPathArt , kArtFullySelected , kArtFullySelected },
        { kPlacedArt , kArtFullySelected , kArtFullySelected },
        { kRasterArt , kArtFullySelected , kArtFullySelected },
        { kPluginArt , kArtFullySelected , kArtFullySelected },
        { kMeshArt , kArtFullySelected , kArtFullySelected },
        { kTextFrameArt , kArtFullySelected , kArtFullySelected },
        { kSymbolArt , kArtFullySelected , kArtFullySelected },
    };
    AIArtSpec allSpecs[] = { { kPathArt , 0 , 0 },
        { kCompoundPathArt , 0 , 0 },
        { kPlacedArt , 0 , 0 },
        { kRasterArt , 0 , 0 },
        { kPluginArt , 0 , 0 },
        { kMeshArt , 0 , 0 },
        { kTextFrameArt , 0 , 0 },
        { kSymbolArt , 0 , 0 },
    };
    //SELECTION
    if (changeIn == CHANGEIN_SELECTION) { sAIArtSet->MatchingArtSet( selectedSpecs , 8, artSet ); }
    //PAGE or DOCUMENT for now
    if (changeIn == CHANGEIN_DOCUMENT) { sAIArtSet->MatchingArtSet( allSpecs , 8, artSet ); }
}

void adjustColor(AIColor *color, void* userData, AIErr *result, AIBoolean *altered) {
    ReplaceData* data = (ReplaceData*)userData;
    
    *altered = FALSE;
    
    AISwatchRef fromSwatchRef = sAISwatchList->GetSwatchByColor(NULL, data->fromColor);
    AISwatchRef colorSwatchRef = sAISwatchList->GetSwatchByColor(NULL, color);
    
    if (color->kind == kGrayColor ||
        color->kind == kFourColor ||
        color->kind == kCustomColor ||
        color->kind == kThreeColor ||
        color->kind == kNoneColor ) {
        
        if ( ColorIsEqual( *data->fromColor , *color , data->tintsCheckbox  ) ){
            if ( data->fromColor->c.c.tint == color->c.c.tint ) { //IF THE TINTS ARE THE SAME
                if (fromSwatchRef == colorSwatchRef) {
                    
                    color = data->toColor; *altered = TRUE;			//Change to the To color
                }
            } else {										//IF THE TINTS ARE DIFFERENT
                AIColor tempColor = *data->toColor;            //Make a new temporary color that is the same as the ToColor,
                tempColor.c.c.tint = color->c.c.tint;     //except the tint is the same as the object's
                
                AISwatchRef toColorTintSwatch = checkSwatchListForColor(tempColor, .01);
                
                if ( toColorTintSwatch ) {//If the swatch already exists
                    sAISwatchList->GetAIColor(toColorTintSwatch, color);
                    *altered = TRUE; //Send back the color of the existing swatch
                }
                else {
                    //Insert a new Swatch into the main group
                    toColorTintSwatch = sAISwatchList->InsertNthSwatch(NULL, -1);
                    if (toColorTintSwatch) {
                        //Apply the tempColor to the new swatch
                        sAISwatchList->SetAIColor(toColorTintSwatch, &tempColor);
                        *color = tempColor; *altered = TRUE; //Send back tempColor (ToColor with the current Objects Tint)
                    }
                }
            }
        }
    }
}

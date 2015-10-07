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
    
    //FILL THE ART SET BASED ON THE "CHANGE IN" LIST
    fillArtSet( artSet , data->changeinSelect );
    
    //Set the VisitFlags based on the apply to and whatnot
    VisitAIColorFlags controlFlags = kVisitColorsNullFlags; //change universally to direct only
    if ( data->applytoSelect == APPLYTO_STROKES ) {	controlFlags = kVisitColorsUniversally | kVisitColorsStrokesOnly | kVisitGlobalObjectsOnceOnly;  }
    if ( data->applytoSelect == APPLYTO_FILLS ) {	controlFlags = kVisitColorsUniversally | kVisitColorsFillsOnly | kVisitGlobalObjectsOnceOnly;  }
    if ( data->applytoSelect == APPLYTO_FILLSANDSTROKES ) {	controlFlags = kVisitColorsUniversally |  kVisitGlobalObjectsOnceOnly;  }
    
    //LOOP THROUGH THE SET AND CHECK THE STROKES AND FILLS FOR THE COLOR WE're CHANGING from

    size_t count;		sAIArtSet->CountArtSet( artSet, &count );
    
    for ( int i=0 ; i < count ; i++ ) {
        AIArtHandle currArtObj;
        sAIArtSet->IndexArtSet( artSet, i, &currArtObj );
      
        
        /*********** FIND AND REPLACE COLORS *************/
        if ( data->attributeSelect == ATTRIBUTE_COLOR ) {
            
            //do the color replacing
            sAIPathStyle->AdjustObjectAIColors( currArtObj , adjustColor , data , controlFlags , &flag );
           
            //increment counter if a switch was made
            if (flag) { numChanged++; }
            flag = FALSE;
        }
        
        
        /*********** FIND AND REPLACE OVERPRINTING *************/
        if ( data->attributeSelect == ATTRIBUTE_OVERPRINT ) {
            
            //Get the AIColor from the color in the drop down
            //initialize the colors
            AIColor FromColor;
            setColorByName( data->fromSelect , FromColor );
            
            AdjustOverprint(currArtObj, FromColor, data->tintsCheckbox, data->addremoveSelect, data->applytoSelect, &flag);
            
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

bool setColorByName( string name , AIColor &color) {
    if (name.compare("[Registration]") == 0) {
        color.kind = kCustomColor;
        color.c.c.tint = 0;
        sAICustomColor->NewRegistrationColor(&color.c.c.color, 1, 1, 1, 1, kCustomFourColor);
        return TRUE;
    }
    
    string colorName;
    float tint = 0;
    size_t found = 0;
    string buffer;
    
    AISwatchRef swatchRef;
    int numSwatches = 0;
    AIColor tempColor;
    ai::UnicodeString tempName;
    
    found = name.find("%");
    
    if (found != string::npos) {
        buffer = name.substr(0, found);
        tint = .01 * (100 - atoi(buffer.c_str()));
    }
    
    if(found == string::npos) { colorName = name.substr(found+1); }
    else { colorName = name.substr(found+2); }
    
    
    numSwatches = sAISwatchList->CountSwatches( NULL );
    for ( int i=0; i<numSwatches; i++) {
        swatchRef = sAISwatchList->GetNthSwatch( NULL , i );
        sAISwatchList->GetSwatchName( swatchRef, tempName );
        sAISwatchList->GetAIColor( swatchRef, &tempColor );
        if ( tempName == (ai::UnicodeString)colorName ) {
            if ( sAIRealMath->EqualWithinTol(tempColor.c.c.tint, tint, .01) ) {     
                color = tempColor;
            }
        }
    }
    
    return TRUE;
}

void adjustColor(AIColor *color, void* userData, AIErr *result, AIBoolean *altered) {
    ReplaceData* data = (ReplaceData*)userData;
    
    //CREATE THE HANDLES and color specs FOR THE TO AND FROM COLORS
    //initialize the colors
    AIColor FromColor, ToColor;
    setColorByName( data->fromSelect , FromColor );
    setColorByName( data->toSelect , ToColor );

    *altered = FALSE;
    
    AISwatchRef fromSwatchRef = sAISwatchList->GetSwatchByColor(NULL, &FromColor);
    AISwatchRef colorSwatchRef = sAISwatchList->GetSwatchByColor(NULL, color);
    
    
    
    if (color->kind == kGrayColor ||
        color->kind == kFourColor ||
        color->kind == kCustomColor ||
        color->kind == kThreeColor ||
        color->kind == kNoneColor ) {
        
        if ( ColorIsEqual( FromColor , *color , data->tintsCheckbox  ) ){
            if ( FromColor.c.c.tint == color->c.c.tint ) { //IF THE TINTS ARE THE SAME
                if (fromSwatchRef == colorSwatchRef) {
                    
                    *color = ToColor; *altered = TRUE;			//Change to the To color
                }
            } else {										//IF THE TINTS ARE DIFFERENT
                AIColor tempColor = ToColor;            //Make a new temporary color that is the same as the ToColor,
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

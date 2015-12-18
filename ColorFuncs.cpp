/*
 *  ColorFuncs.cpp
 *  FixBlack
 *
 *  Created by Comp Mac 8a on 9/4/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 
#include "ColorFuncs.h"
#include "ReplaceData.h"
#include "IAIRect.h"
#include <regex>

const AIReal TOLERANCE = .002;

AIReal GetTint(AIColor* color)
{
    AIReal tintPercent = 0; //Default to 100% for kThreeColor, kGradient, kPattern, kNoneColor
    if (color->kind == kCustomColor) {
        AICustomColor cColor;
        sAICustomColor->GetCustomColor(color->c.c.color, &cColor);
        
        if ((sAIRealMath->EqualWithinTol(color->c.c.tint, 0, TOLERANCE)) &&
            (cColor.kind == kCustomFourColor &&
          sAIRealMath->EqualWithinTol(cColor.c.f.cyan, 0, TOLERANCE) &&
          sAIRealMath->EqualWithinTol(cColor.c.f.magenta, 0, TOLERANCE) &&
          sAIRealMath->EqualWithinTol(cColor.c.f.yellow, 0, TOLERANCE) &&
          cColor.c.f.black > 0))
        {
            tintPercent = 1 - cColor.c.f.black;
        }
        else {
            tintPercent = color->c.c.tint;
        }
    }
    else if (color->kind == kGrayColor)
    {
        tintPercent = 1 - color->c.g.gray; //Make the rounding work out right
    }
    else if ((color->kind == kFourColor &&
              sAIRealMath->EqualWithinTol(color->c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color->c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color->c.f.yellow, 0, TOLERANCE) &&
              color->c.f.black > 0))
    {
        tintPercent = 1 - color->c.f.black;
    }
    else if (color->kind == kNoneColor) {return 0;}
    
    return sAIRealMath->AIRealMultiple(tintPercent - .005, .01, TRUE); //TRUE will round the value up, actual tint % down
}

bool ColorIsBlack(AIColor* color)
{
    if (color->kind == kCustomColor) {
        ai::UnicodeString name;
        sAICustomColor->GetCustomColorName(color->c.c.color, name);
        
        if (!name.compare(ai::UnicodeString(MICR_BLACK_MAG_COLOR_NAME))) return FALSE;
        
        AICustomColor cColor;
        sAICustomColor->GetCustomColor(color->c.c.color, &cColor);
        
        if (((!name.compare(ai::UnicodeString(BLACK_COLOR_NAME)))) ||
            ((cColor.kind == kCustomFourColor &&
              sAIRealMath->EqualWithinTol(cColor.c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.f.yellow, 0, TOLERANCE) &&
              cColor.c.f.black > 0)) ||
            ((cColor.kind == kCustomThreeColor &&
              sAIRealMath->EqualWithinTol(cColor.c.rgb.red, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.rgb.green, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.rgb.blue, 0, TOLERANCE))) ||
            ((cColor.kind == kCustomLabColor &&
              sAIRealMath->EqualWithinTol(cColor.c.lab.a, 0, (TOLERANCE*255)) &&
              sAIRealMath->EqualWithinTol(cColor.c.lab.b, 0, (TOLERANCE*255)) &&
              cColor.c.lab.l < 100))
           ) {
            return TRUE;
        }
    }
    else if (
             (color->kind == kGrayColor && color->c.g.gray > 0) ||
             (color->kind == kFourColor &&
               sAIRealMath->EqualWithinTol(color->c.f.cyan, 0, TOLERANCE) &&
               sAIRealMath->EqualWithinTol(color->c.f.magenta, 0, TOLERANCE) &&
               sAIRealMath->EqualWithinTol(color->c.f.yellow, 0, TOLERANCE) &&
               color->c.f.black > 0) ||
             (color->kind == kFourColor && ////C=70 M=67 Y=64 K=74
              sAIRealMath->EqualWithinTol(color->c.f.cyan, .7, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color->c.f.magenta, .67, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color->c.f.yellow, .64, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color->c.f.black, .74, TOLERANCE*2.5)) ||
             (color->kind == kFourColor && ////C=75 M=68 Y=67 K=90
              sAIRealMath->EqualWithinTol(color->c.f.cyan, .75, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color->c.f.magenta, .68, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color->c.f.yellow, .67, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color->c.f.black, .90, TOLERANCE*2.5)) ||
             (color->kind == kThreeColor &&
               sAIRealMath->EqualWithinTol(color->c.rgb.red, 0, TOLERANCE) &&
               sAIRealMath->EqualWithinTol(color->c.rgb.green, 0, TOLERANCE) &&
               sAIRealMath->EqualWithinTol(color->c.rgb.blue, 0, TOLERANCE))
             )
    {
        return TRUE;
    }

    return FALSE;
}

bool ColorIsWhite(AIColor* color)
{
    if (color->kind == kCustomColor) {
        ai::UnicodeString name;
        sAICustomColor->GetCustomColorName(color->c.c.color, name);
        
        AICustomColor cColor;
        sAICustomColor->GetCustomColor(color->c.c.color, &cColor);
        
        if (((!name.compare(ai::UnicodeString("White")))) ||
            ((cColor.kind == kCustomFourColor &&
              sAIRealMath->EqualWithinTol(cColor.c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.f.yellow, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.f.black, 0, TOLERANCE))) ||
            ((cColor.kind == kCustomThreeColor &&
              sAIRealMath->EqualWithinTol(cColor.c.rgb.red, 1, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.rgb.green, 1, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(cColor.c.rgb.blue, 1, TOLERANCE))) ||
            ((cColor.kind == kCustomLabColor &&
              sAIRealMath->EqualWithinTol(cColor.c.lab.a, 0, (TOLERANCE*255)) &&
              sAIRealMath->EqualWithinTol(cColor.c.lab.b, 0, (TOLERANCE*255)) &&
              sAIRealMath->EqualWithinTol(cColor.c.lab.l, 100, (TOLERANCE*255))))
           ) {
            return TRUE;
        }
    }
    else if (
             (color->kind == kGrayColor &&
              sAIRealMath->EqualWithinTol(color->c.g.gray, 0, TOLERANCE)) ||
             (color->kind == kFourColor &&
              sAIRealMath->EqualWithinTol(color->c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color->c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color->c.f.yellow, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color->c.f.black, 0, TOLERANCE)) ||
             (color->kind == kThreeColor &&
              sAIRealMath->EqualWithinTol(color->c.rgb.red, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color->c.rgb.green, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color->c.rgb.blue, 0, TOLERANCE))
             )
    {
        return TRUE;
    }

    return FALSE;
}

bool ColorIsPantone(AIColor* color)
{
    if (color->kind == kCustomColor) {
        
        AICustomColor tColor;	sAICustomColor->GetCustomColor( color->c.c.color , &tColor );
        
        //If the swatch name includes "PANTONE" return true
        ai::UnicodeString colorName;
        sAICustomColor->GetCustomColorName(color->c.c.color, colorName);
        if (colorName.caseFind(ai::UnicodeString("PANTONE"), 0) != string::npos) {
            return TRUE;
        }
    }
    return FALSE;
}

AICustomColor GetColorDefinitionFromBook(ai::UnicodeString& colorName, bool& found)
{
    AICustomColor tColor;
    if (colorName.caseFind(ai::UnicodeString("PANTONE"), 0) != string::npos) {
        //Get the definition from the swatch book and replace the current definition
        //If the name matches one in the book, store the attributes in customColor
        if ( sAISwatchLibrary->FindStandardBookColor(colorName, &tColor) ) {
            found = TRUE; return tColor;
        }
        else //If the color says "PANTONE" but can't be found, turn it to PANTONE ### U and look it up
        {
            using namespace std;
            string s = colorName.as_Platform();
            
            regex r("PANTONE ([\\w\\s]+) [CVU]{1,3}");
            smatch result;
            regex_search(s,result, r);
            
            string formattedResult = result[1];
            
            formattedResult = "PANTONE " + formattedResult;
            formattedResult += " U";

            if ( sAISwatchLibrary->FindStandardBookColor(ai::UnicodeString(formattedResult), &tColor) ) {
                colorName = ai::UnicodeString(formattedResult);
                found = TRUE; return tColor;
            }
        }
    }
    found = FALSE; return tColor;
}

bool SetColorByName( string name , AIColor &color) {
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



bool ColorIsEqual ( AIColor color1 , AIColor color2 , bool includeTints ) {
	//GRAY COLOR
	if ((color1.kind == kGrayColor) && (color1.kind == color2.kind)) {
		if (color1.c.g.gray == color2.c.g.gray) { return TRUE; }
	}
	
	//FOUR COLOR
	if ((color1.kind == kFourColor) && (color1.kind == color2.kind)) {
		if ( (color1.c.f.cyan == color2.c.f.cyan) &&
		  (color1.c.f.magenta == color2.c.f.magenta) &&
		  (color1.c.f.yellow == color2.c.f.yellow) &&
		  (color1.c.f.black == color2.c.f.black) ) { return TRUE; }
	}

	//THREE COLOR
	if ((color1.kind == kThreeColor) && (color1.kind == color2.kind)) {
		if ( (color1.c.rgb.red == color2.c.rgb.red) &&
		  (color1.c.rgb.green == color2.c.rgb.green) &&
		  (color1.c.rgb.blue == color2.c.rgb.blue) ) { return TRUE; }
	}
	
	//NONE COLOR
	if ((color1.kind == kNoneColor) && (color1.kind == color2.kind)) { return TRUE; }
	
	//CUSTOM COLOR
	if ((color1.kind == kCustomColor) && (color1.kind == color2.kind)) {
		AICustomColor ccolor1, ccolor2;
		sAICustomColor->GetCustomColor( color1.c.c.color, &ccolor1 );
		sAICustomColor->GetCustomColor( color2.c.c.color, &ccolor2 );
		
		//If ignoreTints is off we don't need to make sure the tint is the same here, just the base color, we'll check the
		//tint later and create new ones. If ignoreTints is on, we'll make sure the tints are the same.
		switch ( includeTints ) {
			case TRUE:
				if ( (color1.c.c.tint != color2.c.c.tint) ) { break; }
			case FALSE:
				//CUSTOM FOUR COLOR
				if ( (ccolor1.kind == kCustomFourColor ) && (ccolor1.kind == ccolor2.kind) ) {
					if ( (ccolor1.c.f.cyan == ccolor2.c.f.cyan) &&
					  (ccolor1.c.f.magenta == ccolor2.c.f.magenta) &&
					  (ccolor1.c.f.yellow == ccolor2.c.f.yellow) &&
					  (ccolor1.c.f.black == ccolor2.c.f.black) ) { return TRUE; }				
				}
				//CUSTOM THREE COLOR
				if ( (ccolor1.kind == kCustomThreeColor ) && (ccolor1.kind == ccolor2.kind) ) {
					if ( (ccolor1.c.rgb.red == ccolor2.c.rgb.red) &&
					  (ccolor1.c.rgb.green == ccolor2.c.rgb.green) &&
					  (ccolor1.c.rgb.blue == ccolor2.c.rgb.blue) ) { return TRUE; }				
				}
				//CUSTOM LAB COLOR
				if ( (ccolor1.kind == kCustomLabColor ) && (ccolor1.kind == ccolor2.kind) ) {
					if ( (ccolor1.c.lab.l == ccolor2.c.lab.l) &&
					  (ccolor1.c.lab.a == ccolor2.c.lab.a) &&
					  (ccolor1.c.lab.b == ccolor2.c.lab.b) ) { return TRUE; }				
				}
		}
	}
	return FALSE;
}


void nameAllColors(AIColor *color, void* userData, AIErr *result, AIBoolean *altered) {
		
	if ( sAISwatchList->GetSwatchByColor( NULL , color ) )		return;
	else {
		AISwatchRef newSwatch = sAISwatchList->InsertNthSwatch( NULL , -1 );
		sAISwatchList->SetAIColor(newSwatch, color);
	}
	return;
}



AISwatchRef checkSwatchListForColor( AIColor matchColor , AIReal tolerance ) {
	AISwatchRef currSwatch;
	AIColor currColor;
	int numSwatches = sAISwatchList->CountSwatches(NULL);
	
	//Loop through the swatches to see if the swatch is already there
	for (int i = 0 ; i < numSwatches ; i++ ) {
		currSwatch = sAISwatchList->GetNthSwatch(NULL, i);
		if (currSwatch) {
			sAISwatchList->GetAIColor(currSwatch, &currColor);
			AIBoolean TintsCloseEnough = sAIRealMath->EqualWithinTol(currColor.c.c.tint, matchColor.c.c.tint, tolerance);
			AIBoolean ColorsSame = ColorIsEqual(currColor,matchColor,TRUE);
			if (currColor.kind == kCustomColor && ColorsSame && TintsCloseEnough ) {
				return currSwatch;
			}
		}
	}
	return NULL;
}


void AdjustOverprint(AIArtHandle currArtObj, AIColor color, AIBoolean includeTints, AIBoolean overprint, int replaceIn, AIBoolean *altered) {
	
	AIPathStyle currPathStyle;
	short type = 0; sAIArt->GetArtType(currArtObj, &type);
	*altered = FALSE;
	
	if (type != kTextFrameArt) {
		sAIPathStyle->GetPathStyle(currArtObj, &currPathStyle);
		
		if (type == kPathArt) {
			//Check if its part of a compound path and skip if it is
			int attr = 0;
			sAIArt->GetArtUserAttr(currArtObj, kArtPartOfCompound, &attr);
			if (attr & kArtPartOfCompound) {
				return;
			}
		}
		//STROKES
		if (replaceIn == APPLYTO_STROKES || replaceIn == APPLYTO_FILLSANDSTROKES) {
			if (currPathStyle.strokePaint) {
				if ( ColorIsEqual (currPathStyle.stroke.color, color , includeTints ) ) {
					currPathStyle.stroke.overprint = overprint;
					*altered = TRUE;
				}
			} 
		}
		//FILLS
		if (replaceIn == APPLYTO_FILLS || replaceIn == APPLYTO_FILLSANDSTROKES) {
			if (currPathStyle.fillPaint) {
				if ( ColorIsEqual (currPathStyle.fill.color, color , includeTints ) ) {
					currPathStyle.fill.overprint = overprint;
					*altered = TRUE;
				}
			} 
		}
		
		if (*altered) {
			sAIPathStyle->SetPathStyle(currArtObj, &currPathStyle);
		}		
	} else {
		ATE::TextRangeRef currRangeRef = NULL;
		sAITextFrame->GetATETextRange(currArtObj, &currRangeRef);
		ATE::ITextRange currRange(currRangeRef);
		
		ATE::ITextRunsIterator iter = currRange.GetTextRunsIterator();
		bool isAssigned = FALSE;
		AIColor textColor;
		
		while ( ! iter.IsDone() ) {
			ATE::ICharFeatures currRunCharFeatures = iter.Item().GetUniqueCharFeatures();
			
			
			//STROKES
			if (replaceIn == APPLYTO_STROKES || replaceIn == APPLYTO_FILLSANDSTROKES) {
				ATE::IApplicationPaint strokePaint = currRunCharFeatures.GetStrokeColor(&isAssigned);
				if (isAssigned) {
					sAIATEPaint->GetAIColor(strokePaint.GetRef(), &textColor);
					if ( ColorIsEqual (textColor, color , includeTints ) ) {
						currRunCharFeatures.SetStrokeOverPrint(overprint);
						iter.Item().SetLocalCharFeatures(currRunCharFeatures);
						*altered = TRUE;
					}
				}
			}
			//FILLS
			if (replaceIn == APPLYTO_FILLS || replaceIn == APPLYTO_FILLSANDSTROKES) {
				ATE::IApplicationPaint fillPaint = currRunCharFeatures.GetFillColor(&isAssigned);
				if (isAssigned) {
					sAIATEPaint->GetAIColor(fillPaint.GetRef(), &textColor);
					if ( ColorIsEqual (textColor, color , includeTints ) ) {
						currRunCharFeatures.SetFillOverPrint(overprint);
						iter.Item().SetLocalCharFeatures(currRunCharFeatures);
						*altered = TRUE;
						
					}
				}
			}
			
			iter.Next();
		}
	}
}


void ChangeColor(AIColor *targetColor, AIColor colorToCopy)
{
	targetColor->kind = colorToCopy.kind;
	targetColor->c.c.tint = colorToCopy.c.c.tint;
	targetColor->c.c.color = colorToCopy.c.c.color;
}

void RemoveWhiteOverprint()
{
    AIColor whiteColor;
    AIArtHandle currArtHandle;
    AIBoolean altered = FALSE;
    
    AICustomColorHandle hWhite = NULL;
    sAICustomColor->GetCustomColorByName(ai::UnicodeString("White"), &hWhite);

    whiteColor.kind = kCustomColor;
    whiteColor.c.c.tint = 1;
    whiteColor.c.c.color = hWhite;
    
    //CREATE THE ART SET
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    
    //FILL THE ART SET
    AIArtSpec artSpecs[] = { { kPathArt , 0 , 0 },
        { kCompoundPathArt , 0 , 0 },
        { kPlacedArt , 0 , 0 },
        { kRasterArt , 0 , 0 },
        { kPluginArt , 0 , 0 },
        { kMeshArt , 0 , 0 },
        { kTextFrameArt , 0 , 0 },
        { kSymbolArt , 0 , 0 },
    };
    sAIArtSet->MatchingArtSet(artSpecs , 8 , artSet );
    
    //LOOP THE ART SET
    size_t count;		sAIArtSet->CountArtSet( artSet, &count );
    for ( int i=0 ; i < count ; i++ ) {
        
        sAIArtSet->IndexArtSet(artSet, i, &currArtHandle);
        
        AdjustOverprint(currArtHandle, whiteColor, TRUE, FALSE, 2, &altered);
        
    }
    //DISPOSE THE ART SET
    sAIArtSet->DisposeArtSet(&artSet);
}

void ConvertObjectsToGlobalCMYK(AIColor *color, void *userData, AIErr *result, AIBoolean *altered)
{
    if (color->kind == kFourColor) {
        if ( color->c.f.cyan != 0 || color->c.f.magenta != 0 || color->c.f.yellow != 0 ) {
            AIColor tempColor; AICustomColor tColor; AICustomColorHandle htColor;
            
            tColor.kind = kCustomFourColor;
            tColor.c.f.cyan = color->c.f.cyan;
            tColor.c.f.magenta = color->c.f.magenta;
            tColor.c.f.yellow = color->c.f.yellow;
            tColor.c.f.black = color->c.f.black;
            tColor.flag = NULL;
            
            ai::UnicodeString CMYKName; char buffer[30];
            CMYKName = ai::UnicodeString("C=");
            sprintf(buffer, "%i", int(tColor.c.f.cyan*100+.5) );
            CMYKName += ai::UnicodeString(buffer);
            CMYKName += ai::UnicodeString(" M=");
            sprintf(buffer, "%i", int(tColor.c.f.magenta*100+.5) );
            CMYKName += ai::UnicodeString(buffer);
            CMYKName += ai::UnicodeString(" Y=");
            sprintf(buffer, "%i", int(tColor.c.f.yellow*100+.5) );
            CMYKName += ai::UnicodeString(buffer);
            CMYKName += ai::UnicodeString(" K=");
            sprintf(buffer, "%i", int(tColor.c.f.black*100+.5) );
            CMYKName += ai::UnicodeString(buffer);
            
            
            AIErr error = sAICustomColor->GetCustomColorByName( CMYKName, &htColor );
            if ( error != kNoErr ) {
                sAICustomColor->NewCustomColor( &tColor , CMYKName , &htColor );
            }
            
            tempColor.kind = kCustomColor;
            tempColor.c.c.tint = 0;
            tempColor.c.c.color = htColor;
            
            //Check swatch list for a close tint
            AISwatchRef tempSwatch = checkSwatchListForColor( tempColor , .01 );
            if ( tempSwatch ) {
                AIColor currColor;
                sAISwatchList->GetAIColor(tempSwatch, &currColor);
                *color = currColor;
                *altered = TRUE; return;
            }
            
            
            *color = tempColor;
            *altered = TRUE; return;
        }
    }
}
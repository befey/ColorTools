/*
 *  ColorFuncs.cpp
 *  FixBlack
 *
 *  Created by Comp Mac 8a on 9/4/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 
#include "ColorFuncs.h"
#include "IAIRect.h"
#include <regex>
#include "GetIllustratorErrorCode.h"
#include "SafeguardFileConstants.h"

const AIReal TOLERANCE = .002;

AIReal GetTint(const AIColor color)
{
    AIReal tintPercent = 0; //Default to 100% for kThreeColor, kGradient, kPattern, kNoneColor
    if (color.kind == kCustomColor)
    {
        AICustomColor cColor;
        sAICustomColor->GetCustomColor(color.c.c.color, &cColor);
        
        if (!ColorIsPantone(color) && ((sAIRealMath->EqualWithinTol(color.c.c.tint, 0, TOLERANCE)) &&
            (cColor.kind == kCustomFourColor &&
          sAIRealMath->EqualWithinTol(cColor.c.f.cyan, 0, TOLERANCE) &&
          sAIRealMath->EqualWithinTol(cColor.c.f.magenta, 0, TOLERANCE) &&
          sAIRealMath->EqualWithinTol(cColor.c.f.yellow, 0, TOLERANCE) &&
          cColor.c.f.black > 0)))
        {
            tintPercent = 1 - cColor.c.f.black;
        }
        else
        {
            tintPercent = color.c.c.tint;
        }
    }
    else if (color.kind == kGrayColor)
    {
        tintPercent = 1 - color.c.g.gray; //Make the rounding work out right
    }
    else if ((color.kind == kFourColor &&
              sAIRealMath->EqualWithinTol(color.c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color.c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color.c.f.yellow, 0, TOLERANCE) &&
              color.c.f.black > 0))
    {
        tintPercent = 1 - color.c.f.black;
    }
    else if (color.kind == kNoneColor)
    {
        return 0;
    }
    
    return sAIRealMath->AIRealMultiple(tintPercent - .005, .01, TRUE); //TRUE will round the value up, actual tint % down
}

bool ColorIsBlack(const AIColor color)
{
    if (color.kind == kCustomColor)
    {
        if (GetColorName(color) == SafeguardFile::MICR_BLACK_MAG_COLOR_NAME)
        {
            return FALSE;
        }
        
        AICustomColor cColor;
        sAICustomColor->GetCustomColor(color.c.c.color, &cColor);
        
        if ((GetColorName(color) == SafeguardFile::BLACK_COLOR_NAME) ||
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
           )
        {
            return TRUE;
        }
    }
    else if (
             (color.kind == kGrayColor && color.c.g.gray > 0) ||
             (color.kind == kFourColor &&
               sAIRealMath->EqualWithinTol(color.c.f.cyan, 0, TOLERANCE) &&
               sAIRealMath->EqualWithinTol(color.c.f.magenta, 0, TOLERANCE) &&
               sAIRealMath->EqualWithinTol(color.c.f.yellow, 0, TOLERANCE) &&
               color.c.f.black > 0) ||
             (color.kind == kFourColor && ////C=70 M=67 Y=64 K=74
              sAIRealMath->EqualWithinTol(color.c.f.cyan, .7, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color.c.f.magenta, .67, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color.c.f.yellow, .64, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color.c.f.black, .74, TOLERANCE*2.5)) ||
             (color.kind == kFourColor && ////C=75 M=68 Y=67 K=90
              sAIRealMath->EqualWithinTol(color.c.f.cyan, .75, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color.c.f.magenta, .68, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color.c.f.yellow, .67, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(color.c.f.black, .90, TOLERANCE*2.5)) ||
             (color.kind == kThreeColor &&
               sAIRealMath->EqualWithinTol(color.c.rgb.red, 0, TOLERANCE) &&
               sAIRealMath->EqualWithinTol(color.c.rgb.green, 0, TOLERANCE) &&
               sAIRealMath->EqualWithinTol(color.c.rgb.blue, 0, TOLERANCE))
             )
    {
        return TRUE;
    }

    return FALSE;
}

bool ColorIsWhite(const AIColor color)
{
    if (color.kind == kCustomColor)
    {
        AICustomColor cColor;
        sAICustomColor->GetCustomColor(color.c.c.color, &cColor);
        
        if ((GetColorName(color) == "White") ||
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
           )
        {
            return TRUE;
        }
    }
    else if (
             (color.kind == kGrayColor &&
              sAIRealMath->EqualWithinTol(color.c.g.gray, 0, TOLERANCE)) ||
             (color.kind == kFourColor &&
              sAIRealMath->EqualWithinTol(color.c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color.c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color.c.f.yellow, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color.c.f.black, 0, TOLERANCE)) ||
             (color.kind == kThreeColor &&
              sAIRealMath->EqualWithinTol(color.c.rgb.red, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color.c.rgb.green, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(color.c.rgb.blue, 0, TOLERANCE))
             )
    {
        return TRUE;
    }

    return FALSE;
}

bool ColorIsGripper(const AIColor color)
{
    if (GetColorName(color) == SafeguardFile::GRIPPER_COLOR_NAME)
    {
        return true;
    }
    return false;
}

bool ColorIsKeyline(const AIColor color)
{
    if (GetColorName(color) == SafeguardFile::KEYLINE_COLOR_NAME)
    {
        return true;
    }
    return false;
}

bool ColorIsPantone(const AIColor color)
{
    //If the swatch name includes "PANTONE" return true
    string s = GetColorName(color);
    ai::UnicodeString colorName(s);
    if (colorName.caseFind(ai::UnicodeString("PANTONE"), 0) != string::npos)
    {
        return TRUE;
    }
    return FALSE;
}

bool ColorIsNonPrinting(const AIColor color)
{
    if (ColorIsWhite(color))
    {
        return true;
    }
/*    if (GetRegistrationColor().c.c.color == color.c.c.color)
    {
        return true;
    }
*/  if (ColorIsGripper(color))
    {
        return true;
    }
    if (ColorIsKeyline(color))
    {
        return true;
    }
    
    return false;
}

AIColor GetColorDefinitionFromBook(string name, bool& found)
{
    found = FALSE;
    AIColor resultColor;
    AICustomColor tColor;
    ai::UnicodeString colorName(name);
    if (colorName.caseFind(ai::UnicodeString("PANTONE"), 0) != string::npos)
    {
        //Get the definition from the swatch book and replace the current definition
        //If the name matches one in the book, store the attributes in customColor
        if ( sAISwatchLibrary->FindStandardBookColor(colorName, &tColor) )
        {
            found = TRUE;
        }
        else //If the color says "PANTONE" but can't be found, turn it to PANTONE ### U and look it up
        {
            string s = colorName.as_Platform();
            
            string formattedResult = GetInnerPantoneColorNumber(s);
            
            formattedResult = "PANTONE " + formattedResult + " U";

            if ( sAISwatchLibrary->FindStandardBookColor(ai::UnicodeString(formattedResult), &tColor) )
            {
                colorName = ai::UnicodeString(formattedResult);
                found = TRUE;
            }
        }
    }
    
    AICustomColorHandle tColorHandle;
    ASErr err = sAICustomColor->NewCustomColor(&tColor, colorName, &tColorHandle);
    string error = GetIllustratorErrorCode(err);
    
    if (found == TRUE)
    {
        if (err == kNameInUseErr)
        {
            sAICustomColor->NewCustomColor(&tColor, ai::UnicodeString("DUMMY NAME"), &tColorHandle);
            sAICustomColor->GetCustomColorByName(ai::UnicodeString(colorName), &tColorHandle);
        }
        
        sAICustomColor->SetCustomColor(tColorHandle, &tColor);
        resultColor = {.kind = kCustomColor, .c.c.color = tColorHandle, .c.c.tint = 0};
        
        return resultColor;
    }
    else
    {
        return resultColor;
    }
}

string GetInnerPantoneColorNumber(string fullName)
{
    std::regex r("PANTONE ([\\w\\s]+) [CVU]{1,3}", std::regex::icase);
    std::smatch result;
    std::regex_search(fullName, result, r);
    
    return result[1];
}

string GetInnerPantoneColorNumber(AIColor color)
{
    if (ColorIsPantone(color))
    {
        return GetInnerPantoneColorNumber(GetColorName(color));
    }
    return "";
}

bool SetColorByName( const string& name , AIColor &color)
{
    if (name.compare("[Registration]") == 0)
    {
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
    
    if (found != string::npos)
    {
        buffer = name.substr(0, found);
        tint = .01 * (100 - atoi(buffer.c_str()));
    }
    
    if(found == string::npos)
    {
        colorName = name.substr(found+1);
    }
    else
    {
        colorName = name.substr(found+2);
    }
    
    
    numSwatches = sAISwatchList->CountSwatches( NULL );
    for ( int i=0; i<numSwatches; i++)
    {
        swatchRef = sAISwatchList->GetNthSwatch( NULL , i );
        sAISwatchList->GetSwatchName( swatchRef, tempName );
        sAISwatchList->GetAIColor( swatchRef, &tempColor );
        if ( tempName == (ai::UnicodeString)colorName )
        {
            if (tempColor.kind != kCustomColor)
            {
                color = tempColor;
                return TRUE;
            }
            else if ( sAIRealMath->EqualWithinTol(tempColor.c.c.tint, tint, .01) )
            {
                color = tempColor;
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

string GetColorName(const AIColor color)
{
    string name;
    ai::UnicodeString nameUS;
    
    if (color.kind == kCustomColor)
    {
        sAICustomColor->GetCustomColorName(color.c.c.color, nameUS);
    }
    else
    {
        int numSwatches = sAISwatchList->CountSwatches( NULL );
        for ( int i=0; i<numSwatches; i++)
        {
            AIColor tempColor;
            AISwatchRef swatchRef = sAISwatchList->GetNthSwatch( NULL , i );
            sAISwatchList->GetAIColor( swatchRef, &tempColor );
            if ( ColorIsEqual(color, tempColor, true) )
            {
                sAISwatchList->GetSwatchName( swatchRef, nameUS );
            }
        }
    }
    
    return nameUS.getInStdString(kAIPlatformCharacterEncoding);;
}

bool ColorIsEqual ( const AIColor& color1 , const AIColor& color2 , const bool ignoreTints )
{
	//GRAY COLOR
	if ((color1.kind == kGrayColor) && (color1.kind == color2.kind))
    {
		if (color1.c.g.gray == color2.c.g.gray) { return TRUE; }
	}
	
	//FOUR COLOR
	if ((color1.kind == kFourColor) && (color1.kind == color2.kind))
    {
		if ( (color1.c.f.cyan == color2.c.f.cyan) &&
		  (color1.c.f.magenta == color2.c.f.magenta) &&
		  (color1.c.f.yellow == color2.c.f.yellow) &&
		  (color1.c.f.black == color2.c.f.black) ) { return TRUE; }
	}

	//THREE COLOR
	if ((color1.kind == kThreeColor) && (color1.kind == color2.kind))
    {
		if ( (color1.c.rgb.red == color2.c.rgb.red) &&
		  (color1.c.rgb.green == color2.c.rgb.green) &&
		  (color1.c.rgb.blue == color2.c.rgb.blue) ) { return TRUE; }
	}
	
	//NONE COLOR
	if ((color1.kind == kNoneColor) && (color1.kind == color2.kind)) { return TRUE; }
	
	//CUSTOM COLOR
	if ((color1.kind == kCustomColor) && (color1.kind == color2.kind))
    {
		AICustomColor ccolor1, ccolor2;
        sAICustomColor->GetCustomColor( color1.c.c.color, &ccolor1 );
        sAICustomColor->GetCustomColor( color2.c.c.color, &ccolor2 );
		
        if (!ignoreTints && (color1.c.c.tint != color2.c.c.tint)) //If we care about tints and they don't match
        {
            return FALSE;
        }
        
        //CUSTOM FOUR COLOR
        if ( (ccolor1.kind == kCustomFourColor ) && (ccolor1.kind == ccolor2.kind) )
        {
            if ( (ccolor1.c.f.cyan == ccolor2.c.f.cyan) &&
                (ccolor1.c.f.magenta == ccolor2.c.f.magenta) &&
                (ccolor1.c.f.yellow == ccolor2.c.f.yellow) &&
                (ccolor1.c.f.black == ccolor2.c.f.black) ) { return TRUE; }
        }
        //CUSTOM THREE COLOR
        if ( (ccolor1.kind == kCustomThreeColor ) && (ccolor1.kind == ccolor2.kind) )
        {
            if ( (ccolor1.c.rgb.red == ccolor2.c.rgb.red) &&
                (ccolor1.c.rgb.green == ccolor2.c.rgb.green) &&
                (ccolor1.c.rgb.blue == ccolor2.c.rgb.blue) ) { return TRUE; }
        }
        //CUSTOM LAB COLOR
        if ( (ccolor1.kind == kCustomLabColor ) && (ccolor1.kind == ccolor2.kind) )
        {
            if ( (ccolor1.c.lab.l == ccolor2.c.lab.l) &&
                (ccolor1.c.lab.a == ccolor2.c.lab.a) &&
                (ccolor1.c.lab.b == ccolor2.c.lab.b) ) { return TRUE; }				
        }
	}
	return FALSE;
}


void nameAllColors(AIColor *color, void* userData, AIErr *result, AIBoolean *altered)
{
	if ( sAISwatchList->GetSwatchByColor( NULL , color ) )
    {
        return;
    }
	else
    {
		AISwatchRef newSwatch = sAISwatchList->InsertNthSwatch( NULL , -1 );
		sAISwatchList->SetAIColor(newSwatch, color);
	}
	return;
}



AISwatchRef checkSwatchListForColor( AIColor& matchColor , AIReal tolerance )
{
    AISwatchRef currSwatch;
    AIColor currColor;
    int numSwatches = sAISwatchList->CountSwatches(NULL);
    
    //Loop through the swatches to see if the swatch is already there
    for (int i = 0 ; i < numSwatches ; i++ )
    {
        currSwatch = sAISwatchList->GetNthSwatch(NULL, i);
        if (currSwatch)
        {
            sAISwatchList->GetAIColor(currSwatch, &currColor);
            string n1 = GetColorName(matchColor);
            string n2 = GetColorName(currColor);
            AIBoolean TintsCloseEnough = sAIRealMath->EqualWithinTol(GetTint(currColor), GetTint(matchColor), tolerance);
            if (n1 == n2)
            {
                AIBoolean ColorsSame = ColorIsEqual(currColor, matchColor, TRUE);
                if (currColor.kind == kCustomColor && ColorsSame && TintsCloseEnough )
                {
                    return currSwatch;
                }
            }
            if (TintsCloseEnough && ((ColorIsBlack(matchColor) && n2 == SafeguardFile::BLACK_COLOR_NAME) || (ColorIsWhite(matchColor) && n2 == SafeguardFile::WHITE_COLOR_NAME)))
            {
                return currSwatch;
            }
        }
    }
    return NULL;
}


void AdjustOverprint(AIArtHandle currArtObj, AIColor fromColor, AIBoolean includeTints, AIBoolean overprint, ColorToolsUIController::ApplyTo replaceIn, AIBoolean *altered)
{
	AIPathStyle currPathStyle;
	short type = 0; sAIArt->GetArtType(currArtObj, &type);
	*altered = FALSE;
	
	if (type != kTextFrameArt)
    {
		sAIPathStyle->GetPathStyle(currArtObj, &currPathStyle);
		
		if (type == kPathArt)
        {
			//Check if its part of a compound path and skip if it is
			int attr = 0;
			sAIArt->GetArtUserAttr(currArtObj, kArtPartOfCompound, &attr);
			if (attr & kArtPartOfCompound)
            {
				return;
			}
		}
		//STROKES
        if (replaceIn == ColorToolsUIController::ApplyTo::Strokes || replaceIn == ColorToolsUIController::ApplyTo::FillsAndStrokes)
        {
			if (currPathStyle.strokePaint)
            {
				if ( ColorIsEqual (currPathStyle.stroke.color, fromColor , includeTints ) )
                {
					currPathStyle.stroke.overprint = overprint;
					*altered = TRUE;
				}
			} 
		}
		//FILLS
        if (replaceIn == ColorToolsUIController::ApplyTo::Fills || replaceIn == ColorToolsUIController::ApplyTo::FillsAndStrokes)
        {
			if (currPathStyle.fillPaint)
            {
				if ( ColorIsEqual (currPathStyle.fill.color, fromColor , includeTints ) )
                {
					currPathStyle.fill.overprint = overprint;
					*altered = TRUE;
				}
			} 
		}
		
		if (*altered)
        {
			sAIPathStyle->SetPathStyle(currArtObj, &currPathStyle);
		}		
	}
    else
    {
		ATE::TextRangeRef currRangeRef = NULL;
		sAITextFrame->GetATETextRange(currArtObj, &currRangeRef);
		ATE::ITextRange currRange(currRangeRef);
		
		ATE::ITextRunsIterator iter = currRange.GetTextRunsIterator();
		bool isAssigned = FALSE;
		AIColor textColor;
		
		while ( ! iter.IsDone() )
        {
			ATE::ICharFeatures currRunCharFeatures = iter.Item().GetUniqueCharFeatures();
			
			//STROKES
            if (replaceIn == ColorToolsUIController::ApplyTo::Strokes || replaceIn == ColorToolsUIController::ApplyTo::FillsAndStrokes)
            {
				ATE::IApplicationPaint strokePaint = currRunCharFeatures.GetStrokeColor(&isAssigned);
				if (isAssigned)
                {
					sAIATEPaint->GetAIColor(strokePaint.GetRef(), &textColor);
					if ( ColorIsEqual (textColor, fromColor , includeTints ) )
                    {
						currRunCharFeatures.SetStrokeOverPrint(overprint);
						iter.Item().SetLocalCharFeatures(currRunCharFeatures);
						*altered = TRUE;
					}
				}
			}
			//FILLS
            if (replaceIn == ColorToolsUIController::ApplyTo::Fills || replaceIn == ColorToolsUIController::ApplyTo::FillsAndStrokes)
            {
				ATE::IApplicationPaint fillPaint = currRunCharFeatures.GetFillColor(&isAssigned);
				if (isAssigned)
                {
					sAIATEPaint->GetAIColor(fillPaint.GetRef(), &textColor);
					if ( ColorIsEqual (textColor, fromColor , includeTints ) )
                    {
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
    for ( int i=0 ; i < count ; i++ )
    {
        sAIArtSet->IndexArtSet(artSet, i, &currArtHandle);
        AdjustOverprint(currArtHandle, whiteColor, TRUE, FALSE, ColorToolsUIController::ApplyTo::FillsAndStrokes, &altered);
    }
    //DISPOSE THE ART SET
    sAIArtSet->DisposeArtSet(&artSet);
}

void ConvertObjectsToGlobalCMYK(AIColor *color, void *userData, AIErr *result, AIBoolean *altered)
{
    if (color->kind == kFourColor)
    {
        if ( color->c.f.cyan != 0 || color->c.f.magenta != 0 || color->c.f.yellow != 0 )
        {
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
            if ( error != kNoErr )
            {
                sAICustomColor->NewCustomColor( &tColor , CMYKName , &htColor );
            }
            
            tempColor.kind = kCustomColor;
            tempColor.c.c.tint = 0;
            tempColor.c.c.color = htColor;
            
            //Check swatch list for a close tint
            AISwatchRef tempSwatch = checkSwatchListForColor( tempColor , .01 );
            if ( tempSwatch )
            {
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

AIColor GetRegistrationColor()
{
    AICustomColorHandle registrationCch;
    sAICustomColor->GetCurrentRegistrationColor(&registrationCch);
    AIColor registration = { .kind = kCustomColor, .c.c = { .color = registrationCch, .tint = 0} };
    return registration;
}

bool ColorIsRegistration(AIColor color)
{
    AIColor regColor = GetRegistrationColor();
    if (color.c.c.color == regColor.c.c.color)
    {
        return true;
    }
    return false;
}

AIColor GetBlackColor()
{
    AICustomColorHandle blackCch;
    sAICustomColor->GetCustomColorByName(ai::UnicodeString(SafeguardFile::BLACK_COLOR_NAME), &blackCch);
    AIColor black = { .kind = kCustomColor, .c.c = { .color = blackCch, .tint = 0} };
    return black;
}

vector<AIColor> GetColorsFromArt(AIArtHandle art)
{
    AIBoolean madeReplacement;
    vector<AIColor> colorsInArt;
    sAIPathStyle->AdjustObjectAIColors(art, GetColorsCallback, (void*)&colorsInArt, kVisitColorsReadOnly | kVisitGlobalObjectsOnceOnly | kVisitColorsSolidOnly | kVisitColorsIncludeRegistration, &madeReplacement);
    return colorsInArt;
}

void GetColorsCallback(AIColor *color, void *userData, AIErr *result, AIBoolean *altered)
{
    vector<AIColor>* colorsInArt = (vector<AIColor>*)userData;
    colorsInArt->push_back(*color);
}

bool operator==(const AIGrayColorStyle& lhs, const AIGrayColorStyle& rhs)
{
    return (lhs.gray == rhs.gray);
}

bool operator==(const AIFourColorStyle& lhs, const AIFourColorStyle& rhs)
{
    return (lhs.cyan == rhs.cyan) && (lhs.yellow == rhs.yellow) && (lhs.magenta == rhs.magenta) && (lhs.black == rhs.black);
}

bool operator==(const AIThreeColorStyle& lhs, const AIThreeColorStyle& rhs)
{
    return (lhs.red == rhs.red) && (lhs.green == rhs.green) && (lhs.blue == rhs.blue);
}
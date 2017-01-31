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
#include <boost/algorithm/string.hpp>
#include "BtColor.h"

AIReal GetTint(const AIColor color)
{
    BtColor btcolor(color);
    AIReal tintPercent = 0; //Default to 100% for kThreeColor, kGradient, kPattern, kNoneColor
    if (btcolor.Kind() == kCustomColor)
    {
        if (!btcolor.IsPantone() && ((sAIRealMath->EqualWithinTol(btcolor.AiColor().c.c.tint, 0, TOLERANCE)) &&
            (btcolor.AiCustomColor().kind == kCustomFourColor &&
          sAIRealMath->EqualWithinTol(btcolor.AiCustomColor().c.f.cyan, 0, TOLERANCE) &&
          sAIRealMath->EqualWithinTol(btcolor.AiCustomColor().c.f.magenta, 0, TOLERANCE) &&
          sAIRealMath->EqualWithinTol(btcolor.AiCustomColor().c.f.yellow, 0, TOLERANCE) &&
          btcolor.AiCustomColor().c.f.black > 0)))
        {
            tintPercent = 1 - btcolor.AiCustomColor().c.f.black;
        }
        else
        {
            tintPercent = btcolor.AiColor().c.c.tint;
        }
    }
    else if (btcolor.Kind() == kGrayColor)
    {
        tintPercent = 1 - btcolor.AiColor().c.g.gray; //Make the rounding work out right
    }
    else if ((btcolor.Kind() == kFourColor &&
              sAIRealMath->EqualWithinTol(btcolor.AiColor().c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(btcolor.AiColor().c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(btcolor.AiColor().c.f.yellow, 0, TOLERANCE) &&
              btcolor.AiColor().c.f.black > 0))
    {
        tintPercent = 1 - btcolor.AiColor().c.f.black;
    }
    else if (btcolor.Kind() == kNoneColor)
    {
        return 0;
    }
    
    return sAIRealMath->AIRealMultiple(tintPercent - .005, .01, true); //TRUE will round the value up, actual tint % down
}

AIColor GetColorDefinitionFromBook(string name, bool& found)
{
    found = false;
    AIColor resultColor;
    AICustomColor tColor;
    ai::UnicodeString colorName(name);
    if (colorName.caseFind(ai::UnicodeString("PANTONE"), 0) != string::npos)
    {
        //Get the definition from the swatch book and replace the current definition
        //If the name matches one in the book, store the attributes in customColor
        if ( sAISwatchLibrary->FindStandardBookColor(colorName, &tColor) )
        {
            found = true;
        }
        else //If the color says "PANTONE" but can't be found, turn it to PANTONE ### U and look it up
        {
            string s = colorName.as_Platform();
            
            string formattedResult = GetInnerPantoneColorNumber(s);
            
            formattedResult = "PANTONE " + formattedResult + " U";

            if ( sAISwatchLibrary->FindStandardBookColor(ai::UnicodeString(formattedResult), &tColor) )
            {
                colorName = ai::UnicodeString(formattedResult);
                found = true;
            }
        }
    }
    
    AICustomColorHandle tColorHandle;
    ASErr err = sAICustomColor->NewCustomColor(&tColor, colorName, &tColorHandle);
    string error = GetIllustratorErrorCode(err);
    
    if (found == true)
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
    
    if (result[1] != "")
    {
        return result[1];
    }
    else
    {
        return fullName;
    }
}

string GetInnerPantoneColorNumber(AIColor color)
{
    BtColor btcolor(color);
    if (btcolor.IsPantone())
    {
        return GetInnerPantoneColorNumber(btcolor.Name());
    }
    return "";
}

SafeguardFile::InkMethod GetInkMethodFromColorName(std::string name)
{
    boost::algorithm::to_upper(name);
    for (auto method : SafeguardFile::InkMethodStrings)
    {
        std::size_t found = name.find(method.second);
        if (found != string::npos)
        {
            return method.first;
        }
    }
    return SafeguardFile::InkMethod::NONE;
}

bool SetColorByName( const string& name , AIColor &color)
{
    if (name.compare("[Registration]") == 0)
    {
        color.kind = kCustomColor;
        color.c.c.tint = 0;
        sAICustomColor->NewRegistrationColor(&color.c.c.color, 1, 1, 1, 1, kCustomFourColor);
        return true;
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
                return true;
            }
            else if ( sAIRealMath->EqualWithinTol(tempColor.c.c.tint, tint, .01) )
            {
                color = tempColor;
                return true;
            }
        }
    }
    
    return false;
}

bool ColorIsEqual ( const AIColor& color1 , const AIColor& color2 , const bool ignoreTints )
{
	//GRAY COLOR
	if ((color1.kind == kGrayColor) && (color1.kind == color2.kind))
    {
		return color1.c.g == color2.c.g;
	}
	
	//FOUR COLOR
	if ((color1.kind == kFourColor) && (color1.kind == color2.kind))
    {
		return color1.c.f == color2.c.f;
	}

	//THREE COLOR
	if ((color1.kind == kThreeColor) && (color1.kind == color2.kind))
    {
		return color1.c.rgb == color2.c.rgb;
	}
	
	//NONE COLOR
	if ((color1.kind == kNoneColor) && (color1.kind == color2.kind)) { return true; }
	
	//CUSTOM COLOR
	if ((color1.kind == kCustomColor) && (color1.kind == color2.kind))
    {
        if (!ignoreTints && (color1.c.c.tint != color2.c.c.tint)) //If we care about tints and they don't match
        {
            return false;
        }
        
        AICustomColor ccolor1, ccolor2;
        sAICustomColor->GetCustomColor( color1.c.c.color, &ccolor1 );
        sAICustomColor->GetCustomColor( color2.c.c.color, &ccolor2 );
        
        //CUSTOM FOUR COLOR
        if ( (ccolor1.kind == kCustomFourColor ) && (ccolor1.kind == ccolor2.kind) )
        {
            return ccolor1.c.f == ccolor2.c.f;
        }
        //CUSTOM THREE COLOR
        if ( (ccolor1.kind == kCustomThreeColor ) && (ccolor1.kind == ccolor2.kind) )
        {
            return ccolor1.c.rgb == ccolor2.c.rgb;
        }
        //CUSTOM LAB COLOR
        if ( (ccolor1.kind == kCustomLabColor ) && (ccolor1.kind == ccolor2.kind) )
        {
            return ccolor1.c.lab == ccolor2.c.lab;
        }
	}
	return false;
}


void NameAllColors(AIColor *color, void* userData, AIErr *result, AIBoolean *altered)
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

AISwatchRef CheckSwatchListForColor( AIColor& matchColor , AIReal tolerance )
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
            string n1 = BtColor(matchColor).Name();
            string n2 = BtColor(currColor).Name();
            AIBoolean TintsCloseEnough = sAIRealMath->EqualWithinTol(GetTint(currColor), GetTint(matchColor), tolerance);
            if (n1 == n2)
            {
                AIBoolean ColorsSame = ColorIsEqual(currColor, matchColor, TRUE);
                if (currColor.kind == kCustomColor && ColorsSame && TintsCloseEnough )
                {
                    return currSwatch;
                }
            }
            if (TintsCloseEnough && ((BtColor(matchColor).IsBlack() && n2 == SafeguardFile::BLACK_COLOR_NAME) || (BtColor(matchColor).IsWhite() && n2 == SafeguardFile::WHITE_COLOR_NAME)))
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
	*altered = false;
	
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
					*altered = true;
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
					*altered = true;
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
		bool isAssigned = false;
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
						*altered = true;
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
						*altered = true;
					}
				}
			}
			iter.Next();
		}
	}
}

void RemoveWhiteOverprint()
{
    AIColor whiteColor;
    AIArtHandle currArtHandle;
    AIBoolean altered = false;
    
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
    size_t count;
    sAIArtSet->CountArtSet( artSet, &count );
    for ( int i = 0; i < count; i++ )
    {
        sAIArtSet->IndexArtSet(artSet, i, &currArtHandle);
        AdjustOverprint(currArtHandle, whiteColor, true, false, ColorToolsUIController::ApplyTo::FillsAndStrokes, &altered);
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
            AISwatchRef tempSwatch = CheckSwatchListForColor( tempColor , .01 );
            if ( tempSwatch )
            {
                AIColor currColor;
                sAISwatchList->GetAIColor(tempSwatch, &currColor);
                *color = currColor;
                *altered = true; return;
            }
            
            *color = tempColor;
            *altered = true; return;
        }
    }
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

bool operator==(const AILabColorStyle& lhs, const AILabColorStyle& rhs)
{
    return (lhs.l == rhs.l) && (lhs.a == rhs.a) && (lhs.b == rhs.b);
}

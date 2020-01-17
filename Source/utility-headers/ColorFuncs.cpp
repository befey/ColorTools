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
#include "BtArtHandle.hpp"
#include "BtAteTextFeatures.h"

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
    Bt::BtColor btcolor(color);
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
    
    
    numSwatches = sAISwatchList->CountSwatches( nullptr );
    for ( int i=0; i<numSwatches; i++)
    {
        swatchRef = sAISwatchList->GetNthSwatch( nullptr , i );
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


void NameAllColors(AIColor *color, void* userData, AIErr *result, AIBoolean *altered)
{
	if ( sAISwatchList->GetSwatchByColor( nullptr , color ) )
    {
        return;
    }
	else
    {
		AISwatchRef newSwatch = sAISwatchList->InsertNthSwatch( nullptr , -1 );
		sAISwatchList->SetAIColor(newSwatch, color);
	}
    
    //Make sure 100% color swatch exists
    Bt::BtColor btColor(*color);
    if (btColor.Tint() > 0)
    {
        btColor.Tint(0); //100%
        AIColor c = btColor.AiColor();
        if (!btColor.CheckSwatchListForColor())
        {
            AISwatchRef newSwatch = sAISwatchList->InsertNthSwatch( nullptr , -1 );
            sAISwatchList->SetAIColor(newSwatch, &c);
        }
    }
	return;
}



void AdjustOverprint(AIArtHandle currArtObj, AIColor fromColor, AIBoolean includeTints, AIBoolean overprint, ColorToolsUIController::ApplyTo replaceIn, AIBoolean *altered)
{
    BtArtHandle btHandle(currArtObj);
    
    *altered = false;
    
    //STROKES
    if (replaceIn == ColorToolsUIController::ApplyTo::Strokes || replaceIn == ColorToolsUIController::ApplyTo::FillsAndStrokes)
    {
        if ( btHandle.StrokeColor().ColorIsEqual(fromColor, includeTints) )
        {
            btHandle.OverprintStroke(overprint);
            *altered = true;
        }
    }
    //FILLS
    if (replaceIn == ColorToolsUIController::ApplyTo::Fills || replaceIn == ColorToolsUIController::ApplyTo::FillsAndStrokes)
    {
        if ( btHandle.FillColor().ColorIsEqual(fromColor , includeTints ) )
        {
            btHandle.OverprintFill(overprint);
            *altered = true;
        }
    }
}

void RemoveWhiteOverprint()
{
    AIArtHandle currArtHandle;
    AIBoolean altered = false;
    
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
        AdjustOverprint(currArtHandle, Bt::BtStandardColors().White(), true, false, ColorToolsUIController::ApplyTo::FillsAndStrokes, &altered);
    }
    //DISPOSE THE ART SET
    sAIArtSet->DisposeArtSet(&artSet);
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

/*
 *  ColorFuncs.h
 *  FixBlack
 *
 *  Created by Comp Mac 8a on 9/4/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef COLORFUNCS_H
#define COLORFUNCS_H


//=================================
// included dependencies
#include "IllustratorSDK.h"
#include "AISwatchList.h"
#include "AIATEPaint.h"
#include "AISwatchLibraries.h"
#include "ReplaceData.h"
#include "SafeguardFileConstants.h"
#include <string.h>
#include <vector>

//=================================
// forward declared dependencies
extern AICustomColorSuite *sAICustomColor;
extern AISwatchListSuite *sAISwatchList;
extern AIArtSuite *sAIArt;
extern AIArtSetSuite *sAIArtSet;
extern AIPathStyleSuite *sAIPathStyle;
extern AISwatchGroupSuite *sAISwatchGroup;
extern AISwatchLibrariesSuite *sAISwatchLibrary;
extern AIRealMathSuite *sAIRealMath;
extern AITextFrameSuite *sAITextFrame;
extern AIATEPaintSuite *sAIATEPaint;


//=================================
// Constant definitions

AIReal GetTint(const AIColor color); //Returns the tint of the color to the closest 1%
bool ColorIsBlack(const AIColor color);
bool ColorIsWhite(const AIColor color);
bool ColorIsGripper(const AIColor color);
bool ColorIsPantone(const AIColor color); //Returns true if the color name includes PANTONE
bool ColorIsNonPrinting(const AIColor color);

AIColor GetColorDefinitionFromBook(string colorName, bool& found); //colorName must be a full, valid pantone color in the form "PANTONE ### U", returns the color definition from the book. found indicates whether it was or not. If the name includes PANTONE, but it can't be found, looks up the U version instead, colorName will be the name that matches the definition found

bool SetColorByName( const string& name , AIColor &color);
string GetColorName(const AIColor color);
string GetInnerPantoneColorNumber(string fullName);
string GetInnerPantoneColorNumber(AIColor color);
SafeguardFile::InkMethod GetInkMethodFromColorName(std::string name);

bool ColorIsEqual (const AIColor& color1, const AIColor& color2 , const bool includeTints ); //returns TRUE if they're the same color


AISwatchRef checkSwatchListForColor( AIColor& matchColor , AIReal tolerance ); //Checks the swatchlist for a swatch that matches the color passed
																		//Returns the close swatch if found, or NULL if theres none    Tolerance =.01 for close match

void nameAllColors(AIColor *color, void *userData, AIErr *result, AIBoolean *altered);

void RemoveWhiteOverprint();
void AdjustOverprint(AIArtHandle currArtObj, AIColor fromColor, AIBoolean includeTints, AIBoolean overprint, ColorToolsUIController::ApplyTo replaceIn, AIBoolean *altered);
	// currArtObj - Handle to the art object to adjust
	// fromColor - the color to adjust
	// includeTints - TRUE to change all tints of color   FALSE to only change the color selected and no tints
	// overprint - true to turn overprint on, false to turn off
	// replaceIn - 1 = strokes  2 = fills  0 = strokes and fills
	// altered - flag to show if a change was made
	
void ChangeColor(AIColor *targetColor, AIColor colorToCopy);
void ConvertObjectsToGlobalCMYK(AIColor *color, void *userData, AIErr *result, AIBoolean *altered);

AIColor GetRegistrationColor();
bool ColorIsRegistration(AIColor color);
AIColor GetBlackColor();

vector<AIColor> GetColorsFromArt(AIArtHandle art);
void GetColorsCallback(AIColor *color, void *userData, AIErr *result, AIBoolean *altered);

bool operator==(const AIGrayColorStyle& lhs, const AIGrayColorStyle& rhs);
bool operator==(const AIFourColorStyle& lhs, const AIFourColorStyle& rhs);
bool operator==(const AIThreeColorStyle& lhs, const AIThreeColorStyle& rhs);
	
#endif //COLORFUNCS_H
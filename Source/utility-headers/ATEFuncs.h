/*
 *  ATEFuncs.h
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 11/17/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef ATEFUNCS_H
#define ATEFUNCS_H

#include "string.h"
#include "AIATEPaint.h"
#include "AIFont.h"
#include "AITextFrame.h"
#include <functional>

extern AITextFrameSuite* sAITextFrame;
extern AIArtSuite* sAIArt;
extern AIATEPaintSuite* sAIATEPaint;
extern AIFontSuite* sAIFont;

class BtAteTextFeatures;

//CONSTANT DEFINITIONS
#define WHITESPACES " \t\f\v\n\r"


ai::UnicodeString GetNameFromATETextRange(ATE::ITextRange targetRange);

ASReal GetFontSizeFromAITextFrame(AIArtHandle textFrame);
//This function assumes the art handle passed is a valid textFrame, will return 0 if it is not
//Returns the first font size encountered in the text frame

//Takes the passed TextFrame art object and passes each text run to the passed callback function
bool ProcessTextFrameArt(AIArtHandle textFrame, std::function<bool(ATE::ITextRange)> callback);

//Retrieves the color of a Text Range
// 0 = FILL,     1 = STROKE    DEFAULT = FILL
AIColor GetAIColorFromATETextRange(ATE::ITextRange theRange, bool fillOrStroke = 0);

//Sets the color of a Text Range
void SetAIColorForATETextRange(ATE::ITextRange theRange, AIColor theColor, bool fillOrStroke = 0);

string GetFontNameFromFeatures(const BtAteTextFeatures features);
string GetPostscriptFontNameFromFeatures(const BtAteTextFeatures features);
string GetDisplayFontNameFromPostscriptFontName(const string postscriptFontName);

void AddTextToRange(const string text, ATE::ITextRange& targetRange, int beforeAfter=1);
void AddTextToRange(ATE::ITextRange sourceRange, ATE::ITextRange& targetRange, int beforeAfter=1);

#endif
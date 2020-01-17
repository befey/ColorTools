/*
 *  ATEFuncs.cpp
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 11/17/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ATEFuncs.h"
#include "BtAteTextFeatures.h"
#include "ArtTree.h"

ai::UnicodeString GetNameFromATETextRange(ATE::ITextRange targetRange) {
	ATE::ITextFramesIterator itemFrameIterator = targetRange.GetTextFramesIterator();
	AIArtHandle itemArtHandle = nullptr;
	ATE::ITextFrame itemTextFrame = itemFrameIterator.Item();
	ATE::TextFrameRef itemTextFrameRef = itemTextFrame.GetRef();
	sAITextFrame->GetAITextFrame(itemTextFrameRef, &itemArtHandle);
	ai::UnicodeString itemName;
	sAIArt->GetArtName(itemArtHandle, itemName, nullptr);

	return itemName;
}

ASReal GetFontSizeFromAITextFrame(AIArtHandle textFrame) {
	short type = 0;
	sAIArt->GetArtType(textFrame, &type);
	
	if ( type == kTextFrameArt ) {
		ATE::TextRangeRef currATETextRangeRef;
		sAITextFrame->GetATETextRange(textFrame, &currATETextRangeRef);
		ATE::ITextRange currTextRange(currATETextRangeRef);
		
		ATE::ICharInspector inspector = currTextRange.GetCharInspector();
		ATE::IArrayReal fontsizeArray = inspector.GetFontSize();
		return fontsizeArray.GetFirst();
	} else {
		return 0;
	}
	
}

bool ProcessTextFrameArt(AIArtHandle textFrame, std::function<bool(ATE::ITextRange)> callback) {
	
	short type = 0;
	sAIArt->GetArtType(textFrame, &type);
	if (type != kTextFrameArt) {
		return false;
	}
	
	//Create the ATE range
	ATE::TextRangeRef currATETextRangeRef;
	sAITextFrame->GetATETextRange(textFrame, &currATETextRangeRef);
	ATE::ITextRange currTextRange(currATETextRangeRef);
	
    ATE::ITextRunsIterator it = currTextRange.GetTextRunsIterator();
    
    while (it.IsNotDone())
    {
        callback(it.Item());
        it.Next();
    }
    
	return true;
}

void SetAIColorForATETextRange(ATE::ITextRange theRange, AIColor theColor, bool fillOrStroke /*DEFAULT 0*/) {

	ATE::ICharFeatures targetICharFeatures;
	
	//Change the AIColor to the ATE Color
	ATE::ApplicationPaintRef ateApplicationPaintRef = nullptr;
	sAIATEPaint->CreateATEApplicationPaint(&theColor, &ateApplicationPaintRef);
	ATE::IApplicationPaint targetIApplicationPaint(ateApplicationPaintRef);
	
	//Change the fill or stroke
	if (fillOrStroke == 0) { //FILL
		targetICharFeatures.SetFillColor(targetIApplicationPaint);	
		targetICharFeatures.SetFill(TRUE);
	} else { //STROKE
		targetICharFeatures.SetStrokeColor(targetIApplicationPaint);
		targetICharFeatures.SetStroke(TRUE);		
	}
	
	//Add the features back to the range
	theRange.ReplaceOrAddLocalCharFeatures(targetICharFeatures);
}

AIColor GetAIColorFromATETextRange(ATE::ITextRange theRange, bool fillOrStroke /*DEFAULT 0*/) {
	
	bool isAssigned = FALSE;
	AIColor theColor;
	AIColor *ptheColor = &theColor;
	
	ATE::ICharFeatures targetICharFeatures = theRange.GetUniqueCharFeatures();
	
	ATE::IApplicationPaint targetIApplicationPaint;
	if (fillOrStroke == 0) { //FILL
		targetIApplicationPaint = targetICharFeatures.GetFillColor(&isAssigned);
		theColor.kind = kNoneColor;
	} else { //STROKE
		targetIApplicationPaint = targetICharFeatures.GetStrokeColor(&isAssigned);
		theColor.kind = kNoneColor;
	}
	
	if(isAssigned) {
		sAIATEPaint->GetAIColor(targetIApplicationPaint.GetRef(), &theColor);
	}
	
	return *ptheColor;

}

string GetFontNameFromFeatures(const BtAteTextFeatures features)
{
    string fontNameString = "";
    bool isAssigned = false;
    ATE::IFont font = features.Font(&isAssigned);
    if (isAssigned)
    {
        FontRef ref = font.GetRef();
        AIFontKey key;
        sAIFont->FontKeyFromFont(ref, &key);
        char fontName[256];
        sAIFont->GetUserFontUIName(key, fontName, 256);
        fontNameString = fontName;
    }
    return fontNameString;
}

string GetPostscriptFontNameFromFeatures(const BtAteTextFeatures features)
{
    string fontNameString = "";
    bool isAssigned = false;
    ATE::IFont font = features.Font(&isAssigned);
    if (isAssigned)
    {
        FontRef ref = font.GetRef();
        AIFontKey key;
        sAIFont->FontKeyFromFont(ref, &key);
        char fontName[256];
        sAIFont->GetPostScriptFontName(key, fontName, 256);
        fontNameString = fontName;
    }
    return fontNameString;
}

string GetDisplayFontNameFromPostscriptFontName(const string postscriptFontName)
{
    string fontNameString = "";
    
    AIFontKey key;
    sAIFont->FindFont(postscriptFontName.c_str(), kAIAnyFontTechnology, kNativeAIScript, false, &key);
    char fontName[256];
    sAIFont->GetUserFontUIName(key, fontName, 256);
    fontNameString = fontName;
    
    return fontNameString;
}

void AddTextToRange(const string text, ATE::ITextRange& targetRange, int beforeAfter)
{
    //We have to create a new point text so we can get a new blank range
    AIArtHandle tempTextHandle = nullptr; AIRealPoint anchor ={0,0};
    
    AIArtHandle prep = GetGroupArtOfFirstEditableLayer();
        
    sAITextFrame->NewPointText(kPlaceInsideOnTop, prep, kHorizontalTextOrientation, anchor, &tempTextHandle);
    ATE::TextRangeRef newTextRangeRef;
    sAITextFrame->GetATETextRange(tempTextHandle, &newTextRangeRef);
    ATE::ITextRange newTextRange(newTextRangeRef);
    
    newTextRange.InsertAfter(ai::UnicodeString(text).as_ASUnicode().c_str());
        
    AddTextToRange(newTextRange, targetRange, beforeAfter);
    
    //Trash our temporary art objects
    sAIArt->DisposeArt(tempTextHandle);
}

void AddTextToRange(ATE::ITextRange sourceRange, ATE::ITextRange& targetRange, int beforeAfter)
{
    //Put the new text in the targetRange
    if (beforeAfter == 1)
    {
        targetRange.InsertAfter(sourceRange);
    }
    else
    {
        targetRange.InsertBefore(sourceRange);
    }
}

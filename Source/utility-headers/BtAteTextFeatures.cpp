//
//  BtAteTextFeatures.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/8/16.
//
//

#include "BtAteTextFeatures.h"

BtAteTextFeatures::BtAteTextFeatures() {};
BtAteTextFeatures::BtAteTextFeatures(ATE::ICharFeatures cFeatures) : charFeatures(ATE::ICharFeatures(cFeatures)) {};
BtAteTextFeatures::BtAteTextFeatures(ATE::IParaFeatures pFeatures) : paraFeatures(ATE::IParaFeatures(pFeatures)) {};
BtAteTextFeatures::BtAteTextFeatures(ATE::ICharFeatures cFeatures, ATE::IParaFeatures pFeatures) : charFeatures(ATE::ICharFeatures(cFeatures)), paraFeatures(ATE::IParaFeatures(pFeatures)) {};

BtAteTextFeatures::BtAteTextFeatures(const BtAteTextFeatures& src) : charFeatures(ATE::ICharFeatures(src.charFeatures)), paraFeatures(ATE::IParaFeatures(src.paraFeatures)) {};

BtAteTextFeatures& BtAteTextFeatures::operator=(const BtAteTextFeatures& rhs)
{
    if (&rhs != this)
    {
        charFeatures = ATE::ICharFeatures(rhs.charFeatures);
        paraFeatures = ATE::IParaFeatures(rhs.paraFeatures);
    }
    return *this;
}

void BtAteTextFeatures::SetLeading(ATETextDOM::Real newVal)
{
    charFeatures.SetLeading(newVal);
}

void BtAteTextFeatures::SetFontSize(ATETextDOM::Real newVal)
{
    charFeatures.SetFontSize(newVal);
}

void BtAteTextFeatures::SetFont(string postscriptFontName)
{
    AIFontKey currFontKey = NULL;
    sAIFont->FindFont(postscriptFontName.c_str(), kAIAnyFontTechnology, kUnknownAIScript, FALSE, &currFontKey);
    FontRef fontRef = NULL;
    sAIFont->FontFromFontKey(currFontKey, &fontRef);
    charFeatures.SetFont(ATE::IFont(ATE::IFont(fontRef)));
}

void BtAteTextFeatures::SetFillColor(AIColor color)
{
    ATE::ApplicationPaintRef paintRef;
    sAIATEPaint->CreateATEApplicationPaint(&color, &paintRef);
    ATE::IApplicationPaint paint(paintRef);
    charFeatures.SetFillColor(paint);
}

void BtAteTextFeatures::SetJustification(ATE::ParagraphJustification newVal)
{
    paraFeatures.SetJustification(newVal);
}

ATE::IFont BtAteTextFeatures::GetFont(bool* isAssigned) const
{
    return charFeatures.GetFont(isAssigned);
}

ATETextDOM::Real BtAteTextFeatures::GetFontSize(bool* isAssigned) const
{
    return charFeatures.GetFontSize(isAssigned);
}

ATETextDOM::Real BtAteTextFeatures::GetLeading( bool* isAssigned) const
{
    return charFeatures.GetLeading(isAssigned);
}


void BtAteTextFeatures::AddTextToRangeWithFeatures(const string text, ATE::ITextRange& targetRange, int beforeAfter)
{
    //We have to create a new point text so we can get a new blank range
    AIArtHandle tempTextHandle = NULL; AIRealPoint anchor ={0,0};
    sAITextFrame->NewPointText(kPlaceAboveAll, NULL, kHorizontalTextOrientation, anchor, &tempTextHandle);
    ATE::TextRangeRef newTextRangeRef;
    sAITextFrame->GetATETextRange(tempTextHandle, &newTextRangeRef);
    ATE::ITextRange newTextRange(newTextRangeRef);
    
    AddTextToRange(text, newTextRange);
    
    AddTextToRangeWithFeatures(newTextRange, targetRange, beforeAfter);
    
    //Trash our temporary art objects
    sAIArt->DisposeArt(tempTextHandle);
    tempTextHandle = NULL;
    
    return;
}

void BtAteTextFeatures::AddTextToRangeWithFeatures(ATE::ITextRange sourceRange, ATE::ITextRange& targetRange, int beforeAfter)
{
    ApplyFeaturesToRange(sourceRange);
    AddTextToRange(sourceRange, targetRange);
    
    return;
}

void BtAteTextFeatures::ApplyFeaturesToRange(ATE::ITextRange& targetRange)
{
    targetRange.ReplaceOrAddLocalCharFeatures(charFeatures);
    targetRange.ReplaceOrAddLocalParaFeatures(paraFeatures);
}

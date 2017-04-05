//
//  BtAteTextFeatures.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/8/16.
//
//

#include "BtAteTextFeatures.h"
#include "ArtTree.h"

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

BtAteTextFeatures& BtAteTextFeatures::Leading(ATETextDOM::Real newVal)
{
    charFeatures.SetLeading(newVal);
    return *this;
}

ATETextDOM::Real BtAteTextFeatures::Leading(bool* isAssigned) const
{
    return charFeatures.GetLeading(isAssigned);
}

BtAteTextFeatures& BtAteTextFeatures::FontSize(ATETextDOM::Real newVal)
{
    charFeatures.SetFontSize(newVal);
    return *this;
}

ATETextDOM::Real BtAteTextFeatures::FontSize(bool* isAssigned) const
{
    return charFeatures.GetFontSize(isAssigned);
}

BtAteTextFeatures& BtAteTextFeatures::Font(string postscriptFontName)
{
    AIFontKey currFontKey = nullptr;
    sAIFont->FindFont(postscriptFontName.c_str(), kAIAnyFontTechnology, kUnknownAIScript, FALSE, &currFontKey);
    FontRef fontRef = nullptr;
    sAIFont->FontFromFontKey(currFontKey, &fontRef);
    charFeatures.SetFont(ATE::IFont(fontRef));
    return *this;
}

ATE::IFont BtAteTextFeatures::Font(bool* isAssigned) const
{
    return charFeatures.GetFont(isAssigned);
}

BtAteTextFeatures& BtAteTextFeatures::NoBreak(bool newVal)
{
    charFeatures.SetNoBreak(newVal);
    return *this;
}

bool BtAteTextFeatures::NoBreak(bool* isAssigned) const
{
    return charFeatures.GetNoBreak(isAssigned);
}

BtAteTextFeatures& BtAteTextFeatures::FillColor(AIColor color)
{
    charFeatures.SetFillColor(CreateIApplicationPaint(color));
    return *this;
}

AIColor BtAteTextFeatures::FillColor() const
{
    bool isAssigned;
    ATE::IApplicationPaint paint = charFeatures.GetFillColor(&isAssigned);
    
    if (isAssigned)
    {
        AIColor color;
        sAIATEPaint->GetAIColor(paint.GetRef(), &color);
        return color;
    }
    
    return AIColor{.kind = kNoneColor};
}

BtAteTextFeatures& BtAteTextFeatures::FillOverPrint(bool overprint)
{
    charFeatures.SetFillOverPrint(overprint);
    return *this;
}

bool BtAteTextFeatures::FillOverPrint() const
{
    bool isAssigned;
    bool overprintOn = charFeatures.GetFillOverPrint(&isAssigned);
    
    if (isAssigned)
    {
        return overprintOn;
    }
    
    return false;
}

BtAteTextFeatures& BtAteTextFeatures::StrokeColor(AIColor color)
{
    charFeatures.SetStrokeColor(CreateIApplicationPaint(color));
    return *this;
}

AIColor BtAteTextFeatures::StrokeColor() const
{
    bool isAssigned;
    ATE::IApplicationPaint paint = charFeatures.GetStrokeColor(&isAssigned);
    
    if (isAssigned)
    {
        AIColor color;
        sAIATEPaint->GetAIColor(paint.GetRef(), &color);
        return color;
    }
    
    return AIColor{.kind = kNoneColor};
}

BtAteTextFeatures& BtAteTextFeatures::StrokeOverPrint(bool overprint)
{
    charFeatures.SetStrokeOverPrint(overprint);
    return *this;
}

bool BtAteTextFeatures::StrokeOverPrint() const
{
    bool isAssigned;
    bool overprintOn = charFeatures.GetStrokeOverPrint(&isAssigned);
    
    if (isAssigned)
    {
        return overprintOn;
    }
    
    return false;
}

BtAteTextFeatures& BtAteTextFeatures::AutoHyphenate(bool newVal)
{
    paraFeatures.SetAutoHyphenate(newVal);
    return *this;
}

bool BtAteTextFeatures::AutoHyphenate() const
{
    bool isAssigned;
    bool currVal = paraFeatures.GetAutoHyphenate(&isAssigned);
    
    if (isAssigned)
    {
        return currVal;
    }
    
    return false;
}

BtAteTextFeatures& BtAteTextFeatures::Justification(ATE::ParagraphJustification newVal)
{
    paraFeatures.SetJustification(newVal);
    return *this;
}

void BtAteTextFeatures::AddTextToRangeWithFeatures(const string text, ATE::ITextRange& targetRange, int beforeAfter)
{
    //We have to create a new point text so we can get a new blank range
    AIArtHandle tempTextHandle;
    
    AIArtHandle prep = GetGroupArtOfFirstEditableLayer();
    sAITextFrame->NewPointText(kPlaceInsideOnTop, prep, kHorizontalTextOrientation, AIRealPoint{0,0}, &tempTextHandle);
    
    ATE::TextRangeRef newTextRangeRef;
    sAITextFrame->GetATETextRange(tempTextHandle, &newTextRangeRef);
    ATE::ITextRange newTextRange(newTextRangeRef);
    
    AddTextToRange(text, newTextRange);
    
    AddTextToRangeWithFeatures(newTextRange, targetRange, beforeAfter);
        
    //Trash our temporary art objects
    sAIArt->DisposeArt(tempTextHandle);
    tempTextHandle = nullptr;
}

void BtAteTextFeatures::AddTextToRangeWithFeatures(ATE::ITextRange sourceRange, ATE::ITextRange& targetRange, int beforeAfter)
{
    ApplyFeaturesToRange(sourceRange);
    AddTextToRange(sourceRange, targetRange);
}

void BtAteTextFeatures::ApplyFeaturesToRange(ATE::ITextRange& targetRange)
{
    targetRange.ReplaceOrAddLocalCharFeatures(charFeatures);
    targetRange.ReplaceOrAddLocalParaFeatures(paraFeatures);
}

ATE::IApplicationPaint BtAteTextFeatures::CreateIApplicationPaint(AIColor color) const
{
    ATE::ApplicationPaintRef paintRef;
    sAIATEPaint->CreateATEApplicationPaint(&color, &paintRef);
    return ATE::IApplicationPaint(paintRef);
}

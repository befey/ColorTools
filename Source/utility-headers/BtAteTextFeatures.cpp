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
BtAteTextFeatures::BtAteTextFeatures(ATE::ITextRange iTextRange) : charFeatures(iTextRange.GetUniqueLocalCharFeatures()), paraFeatures(iTextRange.GetUniqueLocalParaFeatures()) {};
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

BtAteTextFeatures& BtAteTextFeatures::FillStyle(AIFillStyle fillStyle)
{
    FillColor(fillStyle.color);
    FillOverPrint(fillStyle.overprint);
    return *this;
}

AIFillStyle BtAteTextFeatures::FillStyle() const
{
    return AIFillStyle { FillColor(), FillOverPrint() };
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

BtAteTextFeatures& BtAteTextFeatures::StrokeStyle(AIStrokeStyle strokeStyle)
{
    StrokeColor(strokeStyle.color);
    StrokeOverPrint(strokeStyle.overprint);
    StrokeWidth(strokeStyle.width);
    StrokeDash(strokeStyle.dash);
    StrokeCap(strokeStyle.cap);
    StrokeLineJoin(strokeStyle.join);
    StrokeMiterLimit(strokeStyle.miterLimit);
    return *this;
}

AIStrokeStyle BtAteTextFeatures::StrokeStyle() const
{
    return AIStrokeStyle { StrokeColor(), StrokeOverPrint(), StrokeWidth(), StrokeDash(), StrokeCap(), StrokeLineJoin(), StrokeMiterLimit() };
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

BtAteTextFeatures& BtAteTextFeatures::StrokeWidth(AIReal width)
{
    charFeatures.SetLineWidth(width);
    return *this;
}

AIReal BtAteTextFeatures::StrokeWidth() const
{
    AIReal width;
    bool isAssigned;
    width = charFeatures.GetLineWidth(&isAssigned);
    
    if (isAssigned)
    {
        return width;
    }
    
    return kAIRealUnknown;
}

BtAteTextFeatures& BtAteTextFeatures::StrokeDash(AIDashStyle dash)
{
    ATE::ArrayRealRef dst;
    sAIATEPaint->CreateATELineDashArray(&dash, &dst);
    charFeatures.SetLineDashArray(ATE::IArrayReal(dst));
    charFeatures.SetLineDashOffset(dash.offset);
    return *this;
}

AIDashStyle BtAteTextFeatures::StrokeDash() const
{
    AIDashStyle dashStyle;
    bool isAssigned;
    dashStyle.length = charFeatures.GetLineDashArray(&isAssigned).GetSize();
    
    if (isAssigned)
    {
        for (int i = 0; i < dashStyle.length; i++)
        {
            dashStyle.array[i] = charFeatures.GetLineDashArray(&isAssigned).Item(i);
        }
        dashStyle.offset = charFeatures.GetLineDashOffset(&isAssigned);
        
        return dashStyle;
    }
    
    return AIDashStyle();
}

BtAteTextFeatures& BtAteTextFeatures::StrokeCap(AILineCap cap)
{
    ATE::LineCapType linecaptype = ATE::LineCapType(cap);
    charFeatures.SetLineCap(linecaptype);
    return *this;
}

AILineCap BtAteTextFeatures::StrokeCap() const
{
    bool isAssigned;
    ATE::LineCapType linecaptype = charFeatures.GetLineCap(&isAssigned);
    
    if (isAssigned)
    {
        return AILineCap(linecaptype);
    }
    
    return AILineCap();
}

BtAteTextFeatures& BtAteTextFeatures::StrokeLineJoin(AILineJoin join)
{
    ATE::LineJoinType linejointype = ATE::LineJoinType(join);
    charFeatures.SetLineJoin(linejointype);
    return *this;
}

AILineJoin BtAteTextFeatures::StrokeLineJoin() const
{
    bool isAssigned;
    ATE::LineJoinType linejointype = charFeatures.GetLineJoin(&isAssigned);
    
    if (isAssigned)
    {
        return AILineJoin(linejointype);
    }
    
    return AILineJoin();
}

BtAteTextFeatures& BtAteTextFeatures::StrokeMiterLimit(AIReal miter)
{
    charFeatures.SetMiterLimit(miter);
    return *this;
}

AIReal BtAteTextFeatures::StrokeMiterLimit() const
{
    bool isAssigned;
    AIReal miter = charFeatures.GetMiterLimit(&isAssigned);
    
    if (isAssigned)
    {
        return miter;
    }
    
    return kAIRealUnknown;
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

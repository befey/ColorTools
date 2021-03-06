//
//  BtAteTextFeatures.h
//  SafeguardTools
//
//  Created by T431962 on 6/8/16.
//
//

#ifndef __SafeguardTools__BtAteTextFeatures__
#define __SafeguardTools__BtAteTextFeatures__

#include "AIATEPaint.h"
#include "ATESuites.h"
#include "AITextFrame.h"
#include "ATEFuncs.h"

extern AIATEPaintSuite* sAIATEPaint;
extern AIFontSuite* sAIFont;
extern AITextFrameSuite* sAITextFrame;

class BtAteTextFeatures
{
public:
    BtAteTextFeatures();
    BtAteTextFeatures(ATE::ITextRange iTextRange);
    BtAteTextFeatures(ATE::ICharFeatures cFeatures);
    BtAteTextFeatures(ATE::IParaFeatures pFeatures);
    BtAteTextFeatures(ATE::ICharFeatures cFeatures, ATE::IParaFeatures pFeatures);
    BtAteTextFeatures(const BtAteTextFeatures& src);
    BtAteTextFeatures& operator=(const BtAteTextFeatures& rhs);
    
    BtAteTextFeatures& Leading(ATETextDOM::Real newVal);
    ATETextDOM::Real Leading(bool* isAssigned) const;
    
    BtAteTextFeatures& FontSize(ATETextDOM::Real newVal);
    ATETextDOM::Real FontSize(bool* isAssigned) const;
    
    BtAteTextFeatures& Font(string postscriptFontName);
    ATE::IFont Font(bool* isAssigned) const;
    
    BtAteTextFeatures& NoBreak(bool newVal);
    bool NoBreak(bool* isAssigned) const;
    
    BtAteTextFeatures& FillStyle(AIFillStyle fillStyle);
    AIFillStyle FillStyle() const;
    BtAteTextFeatures& FillColor(AIColor color);
    AIColor FillColor() const;
    BtAteTextFeatures& FillOverPrint(bool overprint);
    bool FillOverPrint() const;
    
    BtAteTextFeatures& StrokeStyle(AIStrokeStyle strokeStyle);
    AIStrokeStyle StrokeStyle() const;
    BtAteTextFeatures& StrokeColor(AIColor color);
    AIColor StrokeColor() const;
    BtAteTextFeatures& StrokeOverPrint(bool overprint);
    bool StrokeOverPrint() const;
    BtAteTextFeatures& StrokeWidth(AIReal width);
    AIReal StrokeWidth() const;
    BtAteTextFeatures& StrokeDash(AIDashStyle dash);
    AIDashStyle StrokeDash() const;
    BtAteTextFeatures& StrokeCap(AILineCap cap);
    AILineCap StrokeCap() const;
    BtAteTextFeatures& StrokeLineJoin(AILineJoin join);
    AILineJoin StrokeLineJoin() const;
    BtAteTextFeatures& StrokeMiterLimit(AIReal miter);
    AIReal StrokeMiterLimit() const;
    
    BtAteTextFeatures& AutoHyphenate(bool newVal);
    bool AutoHyphenate() const;
    
    BtAteTextFeatures& Justification(ATE::ParagraphJustification newVal);
    
    //Adds the text to the range with the features, the last argument defaults to After, set to 0 to insert the text before
    void AddTextToRangeWithFeatures(const string text, ATE::ITextRange& targetRange, int beforeAfter=1);
    void AddTextToRangeWithFeatures(ATE::ITextRange sourceRange, ATE::ITextRange& targetRange, int beforeAfter=1);
    
    void ApplyFeaturesToRange(ATE::ITextRange& targetRange);
    
    inline operator ATE::ICharFeatures&(void) {return charFeatures;}
    inline operator ATE::IParaFeatures&(void) {return paraFeatures;}
private:
    ATE::ICharFeatures charFeatures;
    ATE::IParaFeatures paraFeatures;
    
    ATE::IApplicationPaint CreateIApplicationPaint(AIColor color) const;
};

#endif /* defined(__SafeguardTools__BtAteTextFeatures__) */

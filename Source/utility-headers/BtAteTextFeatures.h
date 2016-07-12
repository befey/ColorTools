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
    BtAteTextFeatures(ATE::ICharFeatures cFeatures);
    BtAteTextFeatures(ATE::IParaFeatures pFeatures);
    BtAteTextFeatures(ATE::ICharFeatures cFeatures, ATE::IParaFeatures pFeatures);
    BtAteTextFeatures(const BtAteTextFeatures& src);
    BtAteTextFeatures& operator=(const BtAteTextFeatures& rhs);
    
    void SetLeading(ATETextDOM::Real newVal);
    void SetFontSize(ATETextDOM::Real newVal);
    void SetFont(string postscriptFontName);
    void SetFillColor(AIColor color);
    void SetJustification(ATE::ParagraphJustification newVal);
    
    ATE::IFont GetFont(bool* isAssigned) const;
    ATETextDOM::Real GetFontSize(bool* isAssigned) const;
    ATETextDOM::Real GetLeading(bool* isAssigned) const;
    
    //Adds the text to the range with the features, the last argument defaults to After, set to 0 to insert the text before
    void AddTextToRangeWithFeatures(const string text, ATE::ITextRange& targetRange, int beforeAfter=1);
    void AddTextToRangeWithFeatures(ATE::ITextRange sourceRange, ATE::ITextRange& targetRange, int beforeAfter=1);
    
    void ApplyFeaturesToRange(ATE::ITextRange& targetRange);
    
    inline operator ATE::ICharFeatures&(void) {return charFeatures;}
    inline operator ATE::IParaFeatures&(void) {return paraFeatures;}
private:
    ATE::ICharFeatures charFeatures;
    ATE::IParaFeatures paraFeatures;
};

#endif /* defined(__SafeguardTools__BtAteTextFeatures__) */

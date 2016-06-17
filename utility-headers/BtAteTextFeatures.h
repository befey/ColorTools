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

extern AIATEPaintSuite* sAIATEPaint;
extern AIFontSuite* sAIFont;

class BtAteTextFeatures
{
public:
    inline operator ATE::ICharFeatures&(void) {return charFeatures;}
    inline operator ATE::IParaFeatures&(void) {return paraFeatures;}
private:
    ATE::ICharFeatures charFeatures;
    ATE::IParaFeatures paraFeatures;
};

#endif /* defined(__SafeguardTools__BtAteTextFeatures__) */

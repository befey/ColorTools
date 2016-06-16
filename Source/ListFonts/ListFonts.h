//
//  ListFonts.h
//  SafeguardTools
//
//  Created by T431962 on 6/14/16.
//
//

#ifndef __SafeguardTools__ListFonts__
#define __SafeguardTools__ListFonts__

#include "AIArtSet.h"
#include "AIArtboard.h"
#include "ATEFuncs.h"
#include "AIFont.h"
#include "AIArtboard.h"
#include <vector>

extern AIArtSetSuite* sAIArtSet;
extern AIArtboardSuite* sAIArtboard;
extern AIFontSuite* sAIFont;
extern AIArtboardSuite* sAIArtboard;

class ListFonts
{
public:
    bool PutFontList();

private:
    vector<ATE::ICharFeatures> featuresList;
    
    long MakeArtSetOfPrintingTextArts(AIArtSet const targetSet);
    long MakeVectorOfFontsFromArtSet(AIArtSet const srcArtSet);
    void RemoveDuplicatesFromFeaturesList();
    void WriteVectorOfFontsToArtboard();
    
    std::function<bool(ATE::ITextRange)> func;
    bool GetFontFromITextRange(ATE::ITextRange currRange);
};


#endif /* defined(__SafeguardTools__ListFonts__) */

//
//  ListFonts.h
//  SafeguardTools
//
//  Created by T431962 on 6/14/16.
//
//

#ifndef __SafeguardTools__ListFonts__
#define __SafeguardTools__ListFonts__

#include "BtAteTextFeatures.h"
#include "AIArtSet.h"
#include "AIArtboard.h"
#include "ATEFuncs.h"
#include "AIFont.h"
#include "AIArtboard.h"
#include <vector>
#include <set>
#include <map>

extern AIArtSetSuite* sAIArtSet;
extern AIArtboardSuite* sAIArtboard;
extern AIFontSuite* sAIFont;
extern AIArtboardSuite* sAIArtboard;

constexpr auto HENCE_FONT_LIST_LABEL =             "__hence_font_list__";
constexpr auto PATH_TO_FONTLIST = "../../../Plug-ins.localized/Safeguard/SafeguardTools.aip/Contents/Resources/fontlist.txt";

class ListFonts
{
public:
    bool PutFontList();

private:
    vector<BtAteTextFeatures> featuresList;
    map<string, string> henceFonts; //<Postscript Fontname, Hence Font Code>
    set<string> postscriptFontNamesOnJob;
    vector<string> listofBadFonts;
    
    long MakeArtSetOfPrintingTextArts(AIArtSet const targetSet);
    long MakeVectorOfFontsFromArtSet(AIArtSet const srcArtSet);
    void RemoveDuplicatesFromFeaturesList();
    void FillJobPostscriptFontList();
    AIArtHandle WriteVectorOfFontsToArtboard();
    
    bool GetFontFromITextRange(ATE::ITextRange currRange);
    bool LoadFontListFromFile();
    bool ValidateAgainstFontList();
};


#endif /* defined(__SafeguardTools__ListFonts__) */

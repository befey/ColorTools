//
//  Plate.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__Plate__
#define __SafeguardTools__Plate__

#include "AIFont.h"
#include "PlateNumber.h"
#include <vector>

extern AIFontSuite* sAIFont;

constexpr auto KEYLINE_LAYER =              "KEY LINE";
constexpr auto REG_LAYER =                  "Registration Guide for Proof";
constexpr auto SLUG_LAYER =                 "SLUG";
constexpr auto FOREGROUND_LAYER =           "Foreground";
constexpr auto GUIDE_LAYER =                "Guides";
constexpr auto BACKGROUND_LAYER =           "Background";

static constexpr auto NO_TOKEN_DESIG =      "F";

class Plate
{
public:
    Plate(ai::ArtboardID id);
    Plate(ai::ArtboardID id, string pn);
    
    void GetColorListAsTextRange(ATE::ITextRange&);
    void GetPlateNumberAndDateAsTextRange(ATE::ITextRange&);
    AIRealRect GetArtboardBounds();
    void SetPlateNumber(string pn) { plateNumber = PlateNumber(pn); }
private:
    vector<AIColor> colorList;
    PlateNumber plateNumber;
    AIUserDateTime lastModified;
    ai::ArtboardID artboardIndex;
    
    string CreateToken() const;
    void FillColorList();
};

#endif /* defined(__SafeguardTools__Plate__) */

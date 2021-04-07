//
//  ColorEnumerator.hpp
//  SafeguardTools
//
//  Created by T431962 on 11/30/16.
//
//

#ifndef ColorEnumerator_hpp
#define ColorEnumerator_hpp

#include <vector>
#include "AIPlaced.h"
#include "AIRaster.h"
#include "AIFOConversion.h"
#include "ColorList.h"

extern AIPlacedSuite* sAIPlaced;
extern AIRasterSuite* sAIRaster;
extern AIFOConversionSuite* sAIFOConversion;

class ColorEnumerator
{
public:
    ColorEnumerator(AIArtHandle art);
    
    const ColorList& GetColorList() const { return colorList; };
    
    bool HasRegistrationColor();
private:
    ColorList colorList;
    
    vector<AIColor> GetColorsFromArt(AIArtHandle art);
    vector<AIColor> GetColorsOfPlacedArt(AIArtHandle art);
    vector<AIColor> GetColorsOfRasterArt(AIArtHandle art);
};

void GetColorsCallback(AIColor *color, void *userData, AIErr *result, AIBoolean *altered);
void AIFOColorsCallback(AIFOContentInfoSelector selector, void *info, void *userData);

#endif /* ColorEnumerator_hpp */

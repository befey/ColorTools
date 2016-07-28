//
//  BtSwatchList.h
//  SafeguardTools
//
//  Created by T431962 on 2/20/15.
//
//

#ifndef __SafeguardTools__BtSwatchList__
#define __SafeguardTools__BtSwatchList__

//=================================
// included dependencies
#include <string>
#include "AIArtSet.h"
#include "AISwatchList.h"
#include "AIPathStyle.h"
#include "BtColor.h"

//=================================
// forward declared dependencies
extern AICustomColorSuite* sAICustomColor;
extern AISwatchListSuite* sAISwatchList;
extern AISwatchGroupSuite* sAISwatchGroup;
extern AIPathStyleSuite* sAIPathStyle;
extern AIArtSetSuite* sAIArtSet;



//=================================
// BtSwatchlist - wrapper for the AISwatchList stuff
class BtSwatchList
{
public:
    
    //Behaviors
    void CreateOrConvertToCustomColor(std::string colorName); //colorName must be one of the standard colors defined, see ColorFuncs.h
    void CreateOrConvertToCustomColor(BtColor color);
    void AdjustAllColors();
    
    void RemoveUnusedColors();
    
    std::vector<std::string> GetCurrentSwatchesAsStringVector();
    string GetColorListAsXMLString();
    
    
private:
    //Members    
    
    static void CreateSwatch(std::string name, AIColor color);
    AISwatchRef GetSwatchByName(std::string name) const;
    
    bool ColorHasDefinitionAlready(BtColor color, AIColor* outFoundColor) const;
        //If the return is TRUE, outFoundColor contains the definition of the matching color
    bool SwatchNameExists(std::string name, AIColor* outFoundColor) const;
        //If the return is TRUE, outFoundColor contains the definition of the matching color
    bool CustomColorExists(BtColor color, AIColor* outFoundColor) const;
        //If the return is TRUE, outFoundColor contains the definition of the matching color
    
    //Adjust Colors Callbacks
    static void AdjustAllColorsCallback(AIColor *color, void *userData, AIErr *result, AIBoolean *altered);
};

#endif /* defined(__SafeguardTools__BtSwatchList__) */

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
#include <unordered_map>
#include "AISwatchList.h"
#include "AIPathStyle.h"

#include "BtColor.h"

//=================================
// forward declared dependencies
extern "C" AICustomColorSuite* sAICustomColor;
extern "C" AISwatchListSuite* sAISwatchList;
extern "C" AIPathStyleSuite* sAIPathStyle;



//=================================
// BtSwatchlist - wrapper for the AISwatchList stuff
class BtSwatchList
{
public:
    //Constuctor
    BtSwatchList();
    
    
    //Getters/Setters

    
    //Behaviors
    void CreateOrConvertToCustomColor(std::string colorName); //colorName must be one of the standard colors defined, see ColorFuncs.h
    void CreateOrConvertToCustomColor(BtColor color);
    void AdjustAllColors();
    
    void RemoveUnusedColors();
    
    std::vector<std::string> GetCurrentSwatchesAsStringVector();
    string GetColorListAsXMLString();
    
    
private:
    //Members
    std::unordered_map<std::string, BtColor> stdColorDefinitions;
    
    
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

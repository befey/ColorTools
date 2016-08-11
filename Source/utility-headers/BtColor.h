//
//  BtColor.h
//  SafeguardTools
//
//  Created by T431962 on 2/16/15.
//
//

#ifndef __SafeguardTools__BtColor__
#define __SafeguardTools__BtColor__


//=================================
// included dependencies
#include <string>
#include "AIColor.h"

//=================================
// forward declared dependencies
extern AICustomColorSuite* sAICustomColor;


//=================================
// BtColor - wrapper for a AICustomColor definition
class BtColor
{
public:
    //Constuctor
    BtColor(AIColor aiColor);
    BtColor(AICustomColor aiCustomColor, std::string name, AIReal tint = 0);
    BtColor(AIColorTag kind, AIColorUnion c);
    BtColor(std::string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag, AIReal tint = 0);

    //Getters/Setters
    BtColor& AiColor(AIColor newVal);
    AIColor AiColor() const { return aiColor; };
    BtColor& AiCustomColor(AICustomColor newVal, std::string name, AIReal tint = 0);
    AICustomColor AiCustomColor() const;
    
    BtColor& Name(std::string newVal);
    std::string Name() const;
    AIColorTag Kind() const;
    
    //Behaviors
    
    
private:
    AIColor aiColor;
    AICustomColor aiCustomColor;
    AICustomColorHandle aiCustomColorHandle = NULL;
};


#endif /* defined(__SafeguardTools__BtColor__) */

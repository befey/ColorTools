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
#include "SafeguardFileConstants.h"
#include "ColorFuncs.h"

//=================================
// forward declared dependencies
extern AICustomColorSuite* sAICustomColor;


//=================================
// BtColor - wrapper for a AICustomColor definition
class BtColor
{
public:
    //Constuctor
    BtColor() {};
    BtColor(AIColor aiColor) :  BtColor(aiColor, SafeguardFile::InkMethod::INVAL) {};
    BtColor(AIColor aiColor, SafeguardFile::InkMethod method);
    
    BtColor(AICustomColor aiCustomColor, std::string name) : BtColor(aiCustomColor, name, 0) {};
    BtColor(AICustomColor aiCustomColor, std::string name, AIReal tint) : BtColor(aiCustomColor, name, 0, SafeguardFile::InkMethod::INVAL) {};
    BtColor(AICustomColor aiCustomColor, std::string name, AIReal tint, SafeguardFile::InkMethod method);
    
    BtColor(AIColorTag kind, AIColorUnion c) : BtColor(kind, c, SafeguardFile::InkMethod::INVAL) {};
    BtColor(AIColorTag kind, AIColorUnion c, SafeguardFile::InkMethod method);
    
    BtColor(std::string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag) : BtColor(name, kind, c, flag, 0) {};
    BtColor(std::string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag, AIReal tint) : BtColor(name, kind, c, flag, tint, SafeguardFile::InkMethod::INVAL) {};
    BtColor(std::string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag, AIReal tint, SafeguardFile::InkMethod method);

    //Getters/Setters
    BtColor& AiColor(AIColor newVal, SafeguardFile::InkMethod method);
    AIColor AiColor() const { return aiColor; };
    
    BtColor& AiCustomColor(AICustomColor newVal, std::string name, AIReal tint, SafeguardFile::InkMethod method);
    AICustomColor AiCustomColor() const;
    
    BtColor& Name(std::string newVal);
    std::string Name() const;
    bool CompareName(std::string name) const;
    
    AIColorTag Kind() const { return aiColor.kind; };
    
    BtColor& Method(SafeguardFile::InkMethod newVal) { method = newVal; return *this; };
    SafeguardFile::InkMethod Method() const { return method; };

private:
    AIColor aiColor;
    AICustomColor aiCustomColor;
    AICustomColorHandle aiCustomColorHandle = NULL;
    
    SafeguardFile::InkMethod method = SafeguardFile::InkMethod::INVAL;
};


#endif /* defined(__SafeguardTools__BtColor__) */

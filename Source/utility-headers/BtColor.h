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
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"

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
    
    AIColorTag Kind() const { return aiColor.kind; };
    
    BtColor& Method(SafeguardFile::InkMethod newVal) { method = newVal; return *this; };
    SafeguardFile::InkMethod Method() const { return method; };

private:
    AIColor aiColor;
    AICustomColor aiCustomColor;
    AICustomColorHandle aiCustomColorHandle = NULL;
    
    SafeguardFile::InkMethod method = SafeguardFile::InkMethod::NONE;
    
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& ar)
    {
        std::string colorName;
        if (ColorIsPantone(aiColor))
        {
            colorName = GetInnerPantoneColorNumber(Name());
        }
        else
        {
            colorName = Name();
        }
        ar(
           CEREAL_NVP(colorName),
           cereal::make_nvp("method", Method())
           );
    }
};


#endif /* defined(__SafeguardTools__BtColor__) */

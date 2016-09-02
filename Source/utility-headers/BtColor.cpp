//
//  BtColor.cpp
//  SafeguardTools
//
//  Created by T431962 on 2/16/15.
//
//

#include "BtColor.h"
#include "ColorFuncs.h"
#include "GetIllustratorErrorCode.h"

#define TMPBLACKNAME "CustomBlack"
#define TMPWHITENAME "CustomWhite"

//Constructor
BtColor::BtColor(AIColor aiColor, SafeguardFile::InkMethod method)
{
    AiColor(aiColor, method);
}

BtColor::BtColor(AICustomColor aiCustomColor, std::string name, AIReal tint, SafeguardFile::InkMethod method)
{
    AiCustomColor(aiCustomColor, name, tint, method);
}

BtColor::BtColor(AIColorTag kind, AIColorUnion c, SafeguardFile::InkMethod method)
{
    aiColor.c = c;
    aiColor.kind = kind;
    AiColor(aiColor, method);
}

BtColor::BtColor(string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag, AIReal tint, SafeguardFile::InkMethod method)
{
    aiCustomColor.c = c;
    aiCustomColor.flag = flag;
    aiCustomColor.kind = kind;
    AiCustomColor(aiCustomColor, name, tint, method);
}

//Getters/Setters
BtColor& BtColor::AiColor(AIColor newVal, SafeguardFile::InkMethod method)
{
    aiColor = newVal;
    if (Kind() == kCustomColor)
    {
        sAICustomColor->GetCustomColor(aiColor.c.c.color, &aiCustomColor);
        aiCustomColorHandle = aiColor.c.c.color;
    }
    
    if (method == SafeguardFile::InkMethod::INVAL)
    {
        this->method = GetInkMethodFromColorName(Name());
    }
    else
    {
        this->method = method;
    }
    
    return *this;
}

BtColor& BtColor::AiCustomColor(AICustomColor newVal, std::string name, AIReal tint, SafeguardFile::InkMethod method)
{
    ASErr err = sAICustomColor->GetCustomColorByName(ai::UnicodeString(name),  &aiCustomColorHandle);
    if (err == kNameNotFoundErr)
    {
        sAICustomColor->NewCustomColor(&newVal, ai::UnicodeString(name), &aiCustomColorHandle);
    }
    aiColor.kind = kCustomColor;
    aiColor.c.c.color = aiCustomColorHandle;
    aiColor.c.c.tint = tint;
    
    if (method == SafeguardFile::InkMethod::INVAL)
    {
        this->method = GetInkMethodFromColorName(Name());
    }
    else
    {
        this->method = method;
    }
    
    return *this;
}

AICustomColor BtColor::AiCustomColor() const
{
    if (Kind() == kCustomColor)
    {
        return aiCustomColor;
    }
    else
    {
        return AICustomColor();
    }
}

BtColor& BtColor::Name(std::string newVal)
{
    if (Kind() == kCustomColor)
    {
        sAICustomColor->SetCustomColorName(aiCustomColorHandle, ai::UnicodeString(newVal));
    }
    return *this;
}

std::string BtColor::Name() const
{
    return GetColorName(aiColor);
}

bool BtColor::CompareName(std::string name) const
{
    if (this->Name() == name || GetInnerPantoneColorNumber(this->Name()) == name)
    {
        return true;
    }
    return false;
}
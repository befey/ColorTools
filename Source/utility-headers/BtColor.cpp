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
BtColor::BtColor(AIColor aiColor)
{
    AiColor(aiColor);
}

BtColor::BtColor(AICustomColor aiCustomColor, std::string name, AIReal tint)
{
    AiCustomColor(aiCustomColor, name, tint);
}

BtColor::BtColor(AIColorTag kind, AIColorUnion c)
{
    aiColor.c = c;
    aiColor.kind = kind;
    AiColor(aiColor);
}

BtColor::BtColor(string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag, AIReal tint)
{
    aiCustomColor.c = c;
    aiCustomColor.flag = flag;
    aiCustomColor.kind = kind;
    AiCustomColor(aiCustomColor, name, tint);
}

//Getters/Setters
BtColor& BtColor::AiColor(AIColor newVal)
{
    aiColor = newVal;
    if (Kind() == kCustomColor)
    {
        sAICustomColor->GetCustomColor(aiColor.c.c.color, &aiCustomColor);
        aiCustomColorHandle = aiColor.c.c.color;
    }
    method = GetInkMethodFromColorName(Name());
    return *this;
}

BtColor& BtColor::AiCustomColor(AICustomColor newVal, std::string name, AIReal tint)
{
    ASErr err = sAICustomColor->GetCustomColorByName(ai::UnicodeString(name),  &aiCustomColorHandle);
    if (err == kNameNotFoundErr)
    {
        sAICustomColor->NewCustomColor(&newVal, ai::UnicodeString(name), &aiCustomColorHandle);
    }
    aiColor.kind = kCustomColor;
    aiColor.c.c.color = aiCustomColorHandle;
    aiColor.c.c.tint = tint;
    method = GetInkMethodFromColorName(Name());
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
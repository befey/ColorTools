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
#include "BtAteTextFeatures.h"
#include "SafeguardFileConstants.h"

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

BtColor::BtColor(std::string name)
{
    AiCustomColor(AICustomColor(), name, 0, SafeguardFile::InkMethod::NONE);
}

bool operator< (const BtColor& lhs, const BtColor& rhs)
{
    //Always sort CMYK to the front
    if (lhs.Kind() == kFourColor && rhs.Kind() != kFourColor)
    {
        return true;
    }
    else if (lhs.Kind() != kFourColor && rhs.Kind() == kFourColor)
    {
        return false;
    }
    else if (lhs.Kind() == kFourColor && rhs.Kind() == kFourColor)
    {
        return lhs.AiColor().c.f.black < rhs.AiColor().c.f.black;
    }
    
    //Otherwise sort everything by luminance
    AILabColorStyle labLhs = lhs.GetLabApproximation();
    AILabColorStyle labRhs = rhs.GetLabApproximation();
    
    if (labLhs.l != labRhs.l)
    {
        return labLhs.l < labRhs.l;
    }
    if (labLhs.a != labRhs.a)
    {
        return labLhs.a < labRhs.a;
    }
    else
    {
        return labLhs.b < labRhs.b;
    }
}

AILabColorStyle BtColor::GetLabApproximation() const
{
    if (aiCustomColorHandle && aiCustomColor.kind == kCustomLabColor)
    {
        return aiCustomColor.c.lab;
    }
    if (aiCustomColor.kind == kCustomFourColor || aiColor.kind == kFourColor)
    {
        AIFourColorStyle s;
        if (aiCustomColorHandle)
        {
            s = aiCustomColor.c.f;
        }
        else
        {
            s = aiColor.c.f;
        }
        
        AIFloatSampleComponent values[4] =
        {
            static_cast<AIFloatSampleComponent>(s.cyan),
            static_cast<AIFloatSampleComponent>(s.magenta),
            static_cast<AIFloatSampleComponent>(s.yellow),
            static_cast<AIFloatSampleComponent>(s.black)
        };
        AIFloatSampleComponent result[3];
        AIBoolean inGamut;
        sAIColorConversion->ConvertSampleColor(kAICMYKColorSpace, values, kAILabColorSpace, result, AIColorConvertOptions::Purpose
                                               ::kDefault, &inGamut);
        
        return AILabColorStyle{result[0], result[1], result[2]};
    }
    if (aiCustomColor.kind == kCustomThreeColor || aiColor.kind == kThreeColor)
    {
        AIThreeColorStyle s;
        if (aiCustomColorHandle)
        {
            s = aiCustomColor.c.rgb;
        }
        else
        {
            s = aiColor.c.rgb;
        }
        
        AIFloatSampleComponent values[3] =
        {
            static_cast<AIFloatSampleComponent>(s.red),
            static_cast<AIFloatSampleComponent>(s.green),
            static_cast<AIFloatSampleComponent>(s.blue),
        };
        AIFloatSampleComponent result[3];
        AIBoolean inGamut;
        sAIColorConversion->ConvertSampleColor(kAIRGBColorSpace, values, kAILabColorSpace, result, AIColorConvertOptions::Purpose
                                               ::kDefault, &inGamut);
        
        return AILabColorStyle{result[0], result[1], result[2]};
    }
    if (aiColor.kind == kGrayColor)
    {
        AIGrayColorStyle s = aiColor.c.g;
        
        AIFloatSampleComponent values[1] =
        {
            static_cast<AIFloatSampleComponent>(s.gray),
        };
        AIFloatSampleComponent result[3];
        AIBoolean inGamut;
        sAIColorConversion->ConvertSampleColor(kAIGrayColorSpace, values, kAILabColorSpace, result, AIColorConvertOptions::Purpose
                                               ::kDefault, &inGamut);
        
        return AILabColorStyle{result[0], result[1], result[2]};
    }
    
    return AILabColorStyle{0, 0, 0};
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

BtColor& BtColor::AiCustomColorHandle(AICustomColorHandle newVal)
{
    if (newVal)
    {
        if (aiColor.kind != kCustomColor)
        {
            aiColor.kind = kCustomColor;
            aiColor.c.c.tint = 0;
        }
        
        aiCustomColorHandle = newVal;
        sAICustomColor->GetCustomColor(aiCustomColorHandle, &aiCustomColor);
    }
    
    return *this;
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
    if (name == SafeguardFile::CMYK_COLOR_NAME)
    {
        if (this->AiColor().kind == kFourColor)
        {
            return true;
        }
    }
    else if (this->Name() == name || GetInnerPantoneColorNumber(this->Name()) == name)
    {
        return true;
    }
    return false;
}

void BtColor::GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const
{
    BtAteTextFeatures textFeatures;
    AIColor textFillColor;
    
    if (Kind() == kNoneColor)
    {
        textFeatures.FillColor(GetRegistrationColor());
        textFeatures.AddTextToRangeWithFeatures("NO IMPRINT", targetRange);
    }
    else
    {
        string name;
        string method = "";
        
        if (Method() != SafeguardFile::InkMethod::NONE)
        {
            method = " " + SafeguardFile::InkMethodStrings.at(Method());
        }
        
        if (ColorIsPantone(AiColor()))
        {
            name = GetInnerPantoneColorNumber(AiColor()) + method;
            textFillColor = AiColor();
            textFillColor.c.c.tint = 0;
            textFeatures.FillColor(textFillColor);
        }
        else if (Kind() == kFourColor)
        {
            AIColor c = {.kind = kFourColor, .c.f.cyan = 1, .c.f.magenta = 0, .c.f.yellow = 0, .c.f.black = 0};
            textFeatures.FillColor(c);
            textFeatures.AddTextToRangeWithFeatures("CYAN" + method + "  ", targetRange);
            c = {.kind = kFourColor, .c.f.cyan = 0, .c.f.magenta = 1, .c.f.yellow = 0, .c.f.black = 0};
            textFeatures.FillColor(c);
            textFeatures.AddTextToRangeWithFeatures("MAG" + method + "  ", targetRange);
            c = {.kind = kFourColor, .c.f.cyan = 0, .c.f.magenta = 0, .c.f.yellow = 1, .c.f.black = 0};
            textFeatures.FillColor(c);
            textFeatures.AddTextToRangeWithFeatures("YEL" + method + "  ", targetRange);
            textFeatures.FillColor(GetBlackColor());
            name = GetColorName(GetBlackColor()) + method;
        }
        else
        {
            name = Name() + method;
            textFillColor = AiColor();
            textFillColor.c.c.tint = 0;
            textFeatures.FillColor(textFillColor);
        }
        
        ATETextDOM::Int32 beforeEnd = targetRange.GetEnd();
        textFeatures.NoBreak(true);
        textFeatures.AddTextToRangeWithFeatures((ai::UnicodeString(name).toUpper()).as_Platform(), targetRange);
        textFeatures.NoBreak(false);
        textFeatures.AddTextToRangeWithFeatures("  ", targetRange);
        ATETextDOM::Int32 afterEnd = targetRange.GetEnd();
        
        AIRealRect bounds;
        sAIATETextUtil->GetBoundsFromTextRange(targetRange.GetRef(), &bounds);
        if ( (bounds.right - bounds.left) > maxWidth )
        {
            targetRange.SetEnd(beforeEnd);
            targetRange.InsertAfter(ai::UnicodeString("\n").as_ASUnicode().c_str());
            ATETextDOM::Int32 newEnd = targetRange.GetEnd();
            targetRange.SetEnd(newEnd - beforeEnd + afterEnd);
        }
    }
}

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

bool operator==(const BtColor& lhs, const BtColor& rhs)
{
    if (lhs.Method() != rhs.Method())
    {
        return false;
    }
    
    return ColorIsEqual(lhs.AiColor(), rhs.AiColor(), true);
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

const AICustomColor BtColor::AiCustomColor() const
{
    if (Kind() == kCustomColor)
    {
        return aiCustomColor;
    }
    else
    {
        return AICustomColor(); //Empty custom color
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
        aiColor.c.c.color = aiCustomColorHandle;
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
    ai::UnicodeString nameUS("ERR: COLOR NAME");
    
    if (PrintsAsSpot())
    {
        if (Kind() == kCustomColor)
        {
            sAICustomColor->GetCustomColorName(AiCustomColorHandle(), nameUS);
        }
        else if (IsBlack(false))
        {
            nameUS = ai::UnicodeString(SafeguardFile::BLACK_COLOR_NAME);
        }
        else if (IsWhite())
        {
            nameUS = ai::UnicodeString(SafeguardFile::WHITE_COLOR_NAME);
        }
    }
    else if (PrintsAsProcess())
    {
        nameUS = ai::UnicodeString(SafeguardFile::CMYK_COLOR_NAME);
    }
    else
    {
        int numSwatches = sAISwatchList->CountSwatches( NULL );
        for ( int i=0; i<numSwatches; i++)
        {
            AIColor tempColor;
            AISwatchRef swatchRef = sAISwatchList->GetNthSwatch( NULL , i );
            sAISwatchList->GetAIColor( swatchRef, &tempColor );
            if ( ColorIsEqual(AiColor(), tempColor, true) )
            {
                sAISwatchList->GetSwatchName( swatchRef, nameUS );
            }
        }
    }
    
    return nameUS.getInStdString(kAIPlatformCharacterEncoding);
}

bool BtColor::CompareName(std::string name) const
{
    if (name == SafeguardFile::CMYK_COLOR_NAME)
    {
        if (PrintsAsProcess())
        {
            return true;
        }
    }
    else if (Name() == name || GetInnerPantoneColorNumber(Name()) == name)
    {
        return true;
    }
    return false;
}

AICustomColorFlags BtColor::CustomFlag() const
{
    if (Kind() == kCustomColor)
    {
        return AiCustomColor().flag;
    }
    return 0;
}

BtColor& BtColor::CustomFlag(AICustomColorFlags newVal)
{
    if (Kind() == kCustomColor)
    {
        aiCustomColor.flag = newVal;
        sAICustomColor->SetCustomColor(AiCustomColorHandle(), &aiCustomColor);
    }
    return *this;
}

AIReal BtColor::Tint() const
{
    AIReal tintPercent = 0; //Default to 100% for kThreeColor, kGradient, kPattern, kNoneColor
    if (Kind() == kCustomColor)
    {
        if (!IsPantone() && ((sAIRealMath->EqualWithinTol(AiColor().c.c.tint, 0, TOLERANCE)) &&
                                     (AiCustomColor().kind == kCustomFourColor &&
                                      sAIRealMath->EqualWithinTol(AiCustomColor().c.f.cyan, 0, TOLERANCE) &&
                                      sAIRealMath->EqualWithinTol(AiCustomColor().c.f.magenta, 0, TOLERANCE) &&
                                      sAIRealMath->EqualWithinTol(AiCustomColor().c.f.yellow, 0, TOLERANCE) &&
                                      AiCustomColor().c.f.black > 0)))
        {
            tintPercent = 1 - AiCustomColor().c.f.black;
        }
        else
        {
            tintPercent = AiColor().c.c.tint;
        }
    }
    else if (Kind() == kGrayColor)
    {
        tintPercent = 1 - AiColor().c.g.gray; //Make the rounding work out right
    }
    else if ((Kind() == kFourColor &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.yellow, 0, TOLERANCE) &&
              AiColor().c.f.black > 0))
    {
        tintPercent = 1 - AiColor().c.f.black;
    }
    else if (Kind() == kNoneColor)
    {
        return 0;
    }
    
    return sAIRealMath->AIRealMultiple(tintPercent - .005, .01, true); //TRUE will round the value up, actual tint % down
}

void BtColor::GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const
{
    BtAteTextFeatures textFeatures;
    AIColor textFillColor;
    
    if (Kind() == kNoneColor)
    {
        textFeatures.FillColor(RegistrationColor());
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
        
        if (IsPantone())
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
            textFeatures.FillColor(BlackColor());
            name = BlackColor().Name() + method;
        }
        else
        {
            name = Name() + method;
            textFillColor = AiColor();
            textFillColor.c.c.tint = 0;
            textFeatures.FillColor(textFillColor);
        }
        
        textFeatures.NoBreak(true);
        textFeatures.AddTextToRangeWithFeatures((ai::UnicodeString(name).toUpper()).as_Platform(), targetRange);
        textFeatures.NoBreak(false);
        textFeatures.AddTextToRangeWithFeatures("  ", targetRange);
        
        AIRealRect bounds;
        sAIATETextUtil->GetBoundsFromTextRange(targetRange.GetRef(), &bounds);

    }
}

bool BtColor::PrintsAsProcess() const
{
    return ( Kind() == kFourColor || Kind() == kThreeColor || ( Kind() == kCustomColor && AiCustomColor().flag == 0 ) ) && (!IsBlack(false) && !IsWhite());
}

bool BtColor::PrintsAsSpot() const
{
    return ( Kind() == kCustomColor && AiCustomColor().flag == kCustomSpotColor ) || IsBlack(false) || IsWhite();
}

bool BtColor::IsBlack(bool includeCMYKBuilds) const
{
    if (Kind() == kCustomColor)
    {
        ai::UnicodeString nameUS;
        sAICustomColor->GetCustomColorName(AiCustomColorHandle(), nameUS);
        if (nameUS == ai::UnicodeString(SafeguardFile::MICR_BLACK_MAG_COLOR_NAME))
        {
            return false;
        }
        
        if ((nameUS == ai::UnicodeString(SafeguardFile::BLACK_COLOR_NAME)) ||
            ((AiCustomColor().kind == kCustomFourColor &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.f.yellow, 0, TOLERANCE) &&
              AiCustomColor().c.f.black > 0)) ||
            ((AiCustomColor().kind == kCustomThreeColor &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.rgb.red, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.rgb.green, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.rgb.blue, 0, TOLERANCE))) ||
            ((AiCustomColor().kind == kCustomLabColor &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.lab.a, 0, (TOLERANCE*255)) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.lab.b, 0, (TOLERANCE*255)) &&
              AiCustomColor().c.lab.l < 100))
            )
        {
            return true;
        }
    }
    else if (
             (Kind() == kGrayColor && AiColor().c.g.gray > 0) ||
             (Kind() == kFourColor &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.yellow, 0, TOLERANCE) &&
              AiColor().c.f.black > 0) ||
             (Kind() == kThreeColor &&
              sAIRealMath->EqualWithinTol(AiColor().c.rgb.red, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.rgb.green, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.rgb.blue, 0, TOLERANCE))
             )
    {
        return true;
    }
    else if (includeCMYKBuilds &&
             (
              (Kind() == kFourColor && ////C=70 M=67 Y=64 K=74
              sAIRealMath->EqualWithinTol(AiColor().c.f.cyan, .7, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.magenta, .67, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.yellow, .64, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.black, .74, TOLERANCE*2.5)) ||
             (Kind() == kFourColor && ////C=75 M=68 Y=67 K=90
              sAIRealMath->EqualWithinTol(AiColor().c.f.cyan, .75, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.magenta, .68, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.yellow, .67, TOLERANCE*2.5) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.black, .90, TOLERANCE*2.5))
              )
             )
    {
        return true;
    }
    
    return false;
}

bool BtColor::IsWhite() const
{
    if (Kind() == kCustomColor)
    {
        ai::UnicodeString nameUS;
        sAICustomColor->GetCustomColorName(AiCustomColorHandle(), nameUS);
        if ((nameUS == ai::UnicodeString(SafeguardFile::WHITE_COLOR_NAME)) ||
            ((AiCustomColor().kind == kCustomFourColor &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.f.yellow, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.f.black, 0, TOLERANCE))) ||
            ((AiCustomColor().kind == kCustomThreeColor &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.rgb.red, 1, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.rgb.green, 1, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.rgb.blue, 1, TOLERANCE))) ||
            ((AiCustomColor().kind == kCustomLabColor &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.lab.a, 0, (TOLERANCE*255)) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.lab.b, 0, (TOLERANCE*255)) &&
              sAIRealMath->EqualWithinTol(AiCustomColor().c.lab.l, 100, (TOLERANCE*255))))
            )
        {
            return true;
        }
    }
    else if (
             (Kind() == kGrayColor &&
              sAIRealMath->EqualWithinTol(AiColor().c.g.gray, 0, TOLERANCE)) ||
             (Kind() == kFourColor &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.cyan, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.magenta, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.yellow, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.f.black, 0, TOLERANCE)) ||
             (Kind() == kThreeColor &&
              sAIRealMath->EqualWithinTol(AiColor().c.rgb.red, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.rgb.green, 0, TOLERANCE) &&
              sAIRealMath->EqualWithinTol(AiColor().c.rgb.blue, 0, TOLERANCE))
             )
    {
        return true;
    }
    
    return false;
}

bool BtColor::IsGripper() const
{
    if (Name() == SafeguardFile::GRIPPER_COLOR_NAME)
    {
        return true;
    }
    return false;
}

bool BtColor::IsKeyline() const
{
    if (Name() == SafeguardFile::KEYLINE_COLOR_NAME)
    {
        return true;
    }
    return false;
}

bool BtColor::IsPantone() const
{
    //If the swatch name includes "PANTONE" return true
    ai::UnicodeString colorName(Name());
    if (colorName.caseFind(ai::UnicodeString("PANTONE"), 0) != string::npos)
    {
        return true;
    }
    return false;
}

bool BtColor::IsNonPrinting() const
{
    if (IsWhite())
    {
        return true;
    }
    if (IsGripper())
    {
        return true;
    }
    if (IsKeyline())
    {
        return true;
    }
    
    return false;
}

bool BtColor::IsRegistration() const
{
    AIColor regColor = RegistrationColor();
    if (AiColor().c.c.color == regColor.c.c.color)
    {
        return true;
    }
    return false;
}

BtColor BtColor::RegistrationColor()
{
    AICustomColorHandle registrationCch;
    sAICustomColor->GetCurrentRegistrationColor(&registrationCch);
    AIColor registration = { .kind = kCustomColor, .c.c = { .color = registrationCch, .tint = 0} };
    return BtColor(registration);
}

BtColor BtColor::BlackColor()
{
    AICustomColorHandle blackCch;
    sAICustomColor->GetCustomColorByName(ai::UnicodeString(SafeguardFile::BLACK_COLOR_NAME), &blackCch);
    AIColor black = { .kind = kCustomColor, .c.c = { .color = blackCch, .tint = 0} };
    return BtColor(black);
}

BtColor BtColor::WhiteColor()
{
    AICustomColorHandle whiteCch;
    sAICustomColor->GetCustomColorByName(ai::UnicodeString(SafeguardFile::WHITE_COLOR_NAME), &whiteCch);
    AIColor white = { .kind = kCustomColor, .c.c = { .color = whiteCch, .tint = 0} };
    return BtColor(white);
}

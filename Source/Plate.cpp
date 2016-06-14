//
//  Plate.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "Plate.h"
#include "BtAteTextFeatures.h"
#include "ATEFuncs.h"
#include "ColorFuncs.h"

Plate::Plate(ai::ArtboardID id) : artboardIndex(id) {}
Plate::Plate(ai::ArtboardID id, string pn) : artboardIndex(id), plateNumber(pn) {}

void Plate::GetPlateNumberAndDateAsTextRange(ATE::ITextRange& targetRange)
{
    AIFontKey currFontKey = NULL;
    sAIFont->FindFont("Helvetica-Bold", kAIAnyFontTechnology, kUnknownAIScript, FALSE, &currFontKey);
    FontRef fontRef = NULL;
    sAIFont->FontFromFontKey(currFontKey, &fontRef);
    
    BtAteTextFeatures textFeatures;
    ATE::ICharFeatures(textFeatures).SetFontSize(12.01);
    ATE::ICharFeatures(textFeatures).SetFont(ATE::IFont(fontRef));
    ATE::IParaFeatures(textFeatures).SetJustification(ATE::kRightJustify);
    
    string pndate;
    if (plateNumber.IsValid())
    {
        pndate = plateNumber.GetPlateNumber() + "." + CreateToken();
    }
    int month, year;
    sAIUser->GetMonth(&lastModified, &month);
    sAIUser->GetYear(&lastModified, &year);
    
    pndate += "  " + to_string(month) + "/" + to_string(year);
    
    AddTextToRangeWithFeatures(pndate, textFeatures, &targetRange);
    targetRange.ReplaceOrAddLocalParaFeatures(textFeatures);
    
    SetAIColorForATETextRange(targetRange, GetRegistrationColor());
}

void Plate::GetColorListAsTextRange(ATE::ITextRange& targetRange)
{
    AIFontKey currFontKey = NULL;
    sAIFont->FindFont("Helvetica-Bold", kAIAnyFontTechnology, kUnknownAIScript, FALSE, &currFontKey);
    FontRef fontRef = NULL;
    sAIFont->FontFromFontKey(currFontKey, &fontRef);
    
    BtAteTextFeatures textFeatures;
    ATE::ICharFeatures(textFeatures).SetFontSize(12.01);
    ATE::ICharFeatures(textFeatures).SetFont(ATE::IFont(fontRef));
    ATE::IParaFeatures(textFeatures).SetJustification(ATE::kLeftJustify);
    
    
    
    AddTextToRangeWithFeatures("Color List", textFeatures, &targetRange);
    targetRange.ReplaceOrAddLocalParaFeatures(textFeatures);
}

string Plate::CreateToken() const
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties abProps;
    sAIArtboard->GetArtboardProperties(abList, artboardIndex, abProps);
    ai::UnicodeString abName;
    abProps.GetName(abName);
    string abNameS = abName.getInStdString(kAIPlatformCharacterEncoding);
    
    AIBoolean isDefaultName;
    sAIArtboard->IsDefaultName(abProps, isDefaultName);
    
    if (isDefaultName || abNameS == NO_TOKEN_DESIG)
    {
        return "";
    }
    else
    {
        return abNameS;
    }
}

void Plate::FillColorList()
{
    
}

//
//  BtSwatchList.cpp
//  SafeguardTools
//
//  Created by T431962 on 2/20/15.
//
//

#include "BtSwatchList.h"
#include "ColorFuncs.h"
#include "GetIllustratorErrorCode.h"

using Bt::BtColor;

//Behaviors
void BtSwatchList::FixStdColors()
{
    std::unordered_map<std::string, Bt::BtColor> stdColors = Bt::BtStandardColors().BasicColorDefinitions();
    for ( auto color : stdColors)
    {
        string s = color.second.Name();
        BtColor createdColor = CreateOrConvertToCustomColor(color.second);
        
        AISwatchRef existingSwatchRef = BtSwatchList::GetSwatchByName(s);
        if (existingSwatchRef != nullptr)
        {
            ChangeSwatchColor(existingSwatchRef, createdColor);
        }
        else
        {
            CreateSwatch(createdColor.Name(), createdColor);
        }
    }
    
    std::unordered_map<std::string, Bt::BtColor> sgColors = Bt::BtStandardColors().SafeguardColorDefinitions();
    for ( auto color : sgColors)
    {
        string s = color.second.Name(); //For debugging
        CreateOrConvertToCustomColor(color.second);
    }
}

Bt::BtColor BtSwatchList::CreateOrConvertToCustomColor(std::string colorName)
{
    BtColor createdColor;
    
    Bt::BtStandardColors stdColors;
    
    auto it = stdColors.BasicColorDefinitions().find(colorName);
    if (it != stdColors.BasicColorDefinitions().end())
    {
        createdColor = CreateOrConvertToCustomColor(it->second);
    }
    
    it = stdColors.SafeguardColorDefinitions().find(colorName);
    if (it != stdColors.SafeguardColorDefinitions().end())
    {
        createdColor = CreateOrConvertToCustomColor(it->second);
    }
    
    return createdColor;
}

Bt::BtColor BtSwatchList::CreateOrConvertToCustomColor(BtColor color)
{
    AICustomColor newCustomColorDefinition = color.AiCustomColor();
    
    AIColor newAiColorDefinition;
    AICustomColorHandle hCreatedCustomColor = nullptr;
    
    if ( ColorHasSwatchAlready(color, &newAiColorDefinition) && newAiColorDefinition.kind == kCustomColor)
    {
        newAiColorDefinition.c.c.tint = 0;
        sAICustomColor->SetCustomColor(newAiColorDefinition.c.c.color, &newCustomColorDefinition);
    }
    else
    {
        ASErr err = sAICustomColor->NewCustomColor(&newCustomColorDefinition, ai::UnicodeString(color.Name().append("1")), &hCreatedCustomColor);
        string error = GetIllustratorErrorCode(err);
        newAiColorDefinition.kind = kCustomColor;
        newAiColorDefinition.c.c.tint = 0;
        newAiColorDefinition.c.c.color = hCreatedCustomColor;
    }
    
    return BtColor(newAiColorDefinition);
}

void BtSwatchList::CreateSwatch(string name, BtColor color)
{
        AIColor aiColor = color.AiColor();
        AISwatchRef swatch = sAISwatchList->GetSwatchByName(nullptr, ai::UnicodeString(color.Name()));
        if (swatch == nullptr)
        {
            swatch = sAISwatchList->InsertNthSwatch(nullptr, 2);
        }
    
        ASErr err = sAISwatchList->SetAIColor(swatch, &aiColor);
        sAISwatchList->SetSwatchName(swatch, ai::UnicodeString(color.Name()));
        string error = GetIllustratorErrorCode(err);
}

void BtSwatchList::ChangeSwatchColor(AISwatchRef swatchRef, BtColor color)
{
    AIColor c = color.AiColor();
    sAISwatchList->SetAIColor(swatchRef, &c);
}

void BtSwatchList::AdjustAllColors()
{
    VisitAIColorFlags controlFlags = kVisitColorsSolidOnly | kVisitGlobalObjectsOnceOnly;
    sAIPathStyle->AdjustObjectAIColors( nullptr , AdjustAllColorsCallback , nullptr , controlFlags , nullptr );
    
    RemoveWhiteOverprint();
}

void BtSwatchList::RemoveUnusedColors() {
    int numSwatches = sAISwatchList->CountSwatches(nullptr);
    
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    
    AIArtSpec allSpecs[] = { { kPathArt , 0 , 0 },
        { kCompoundPathArt , 0 , 0 },
        { kPlacedArt , 0 , 0 },
        { kRasterArt , 0 , 0 },
        { kPluginArt , 0 , 0 },
        { kMeshArt , 0 , 0 },
        { kTextFrameArt , 0 , 0 },
        { kSymbolArt , 0 , 0 },
    };
    sAIArtSet->MatchingArtSet( allSpecs , 8, artSet );
    
    size_t count;
    sAIArtSet->CountArtSet( artSet, &count );
    bool used = FALSE;
    
    //Loop through all the swatches in the list
    for(int i=0; i<numSwatches; i++) {
        AISwatchRef currSwatch = sAISwatchList->GetNthSwatch(nullptr, i);
        AIColor currSwatchColor;
        AIColor currColor; AICustomColor currCustomColor;
        used = FALSE;
        AIReal swatchTint = 0;
        
        sAISwatchList->GetAIColor(currSwatch, &currColor);
        
        ai::UnicodeString currSwatchName;
        sAISwatchList->GetSwatchName(currSwatch, currSwatchName);
        sAISwatchList->GetAIColor(currSwatch, &currSwatchColor);
        
        if (currSwatchColor.kind == kCustomColor) {
            swatchTint = currSwatchColor.c.c.tint;
        }
        
        if (currColor.kind == kCustomColor) {
            sAICustomColor->GetCustomColor(currColor.c.c.color, &currCustomColor);
        }
        
        //Don't remove these swatches whether they're used or not
        if( !currSwatchName.compare((ai::UnicodeString)SafeguardFile::NONE_COLOR_NAME) ||  //compare returns 0 if two strings are equal
           !currSwatchName.compare((ai::UnicodeString)SafeguardFile::REGISTRATION_COLOR_NAME) ||
           !currSwatchName.compare((ai::UnicodeString)SafeguardFile::BLACK_COLOR_NAME) ||
           !currSwatchName.compare((ai::UnicodeString)SafeguardFile::WHITE_COLOR_NAME) )
        {
            if (swatchTint == 0) {
                used = TRUE;
            }
        }
        
        //Loop through each art object to see if it uses the current swatch
        for ( int j=0 ; j< count ; j++ ) {
            if(used) {break;}
            AIArtHandle currArtObj = nullptr;
            AIColorUsedHow usage;
            sAIArtSet->IndexArtSet( artSet, j, &currArtObj );
            if (currArtObj) {
                if(currColor.c.c.tint == 0) {
                    sAIPathStyle->ObjectUsesAIColor( currArtObj , &currColor , FALSE , TRUE , &usage);
                } else {
                    sAIPathStyle->ObjectUsesAIColor( currArtObj , &currColor , TRUE , TRUE , &usage);
                }
                if(usage) { used = TRUE; break; }
            } else {
                return;
            }
        }
        
        if (!used) { sAISwatchList->RemoveNthSwatch(nullptr, i); numSwatches--; i--; }
    }
    //DISPOSE OF THE ART SET
    sAIArtSet->DisposeArtSet(&artSet);
    artSet = nullptr;
    
    //Loop through the swatch groups and remove any empty ones
    count = sAISwatchGroup->CountSwatchGroups(nullptr);
    for ( int i=0 ; i<count; i++) {
        AISwatchGroupRef currSwatchGroup = sAISwatchGroup->GetNthSwatchGroup(nullptr, i);
        int currCount = sAISwatchGroup->CountSwatches(currSwatchGroup);
        if (!currCount) { sAISwatchGroup->RemoveSwatchGroup(nullptr, currSwatchGroup, FALSE); i--; count--; }
    }
}


void BtSwatchList::CreateSwatch(std::string name, AIColor color)
{
    AISwatchRef swatch = sAISwatchList->GetSwatchByColor(nullptr, &color);
    if (swatch == nullptr)
    {
        swatch = sAISwatchList->InsertNthSwatch(nullptr, -1);
    }
    sAISwatchList->SetAIColor(swatch, &color);
    if (!name.empty())
    {
        ASErr err = sAISwatchList->SetSwatchName(swatch, ai::UnicodeString(name));
        string error = GetIllustratorErrorCode(err);
    }
}

AISwatchRef BtSwatchList::GetSwatchByName(std::string name)
{
    return sAISwatchList->GetSwatchByName(nullptr, ai::UnicodeString(name));
}

bool BtSwatchList::ColorHasSwatchAlready(BtColor color, AIColor* outFoundColor) const
{
    return SwatchNameExists(color.Name(), outFoundColor) || CustomColorExists(color, outFoundColor);
}

bool BtSwatchList::SwatchNameExists(std::string name, AIColor* outFoundColor) const
{
    AISwatchRef hFoundSwatch = GetSwatchByName(name);
    if (hFoundSwatch != nullptr)
    {
        sAISwatchList->GetAIColor(hFoundSwatch, outFoundColor);
        return TRUE;
    }
    outFoundColor->kind = kNoneColor;
    return FALSE;
}

bool BtSwatchList::CustomColorExists(BtColor color, AIColor* outFoundColor) const
{
    int custColorCount = 0;
    AICustomColorHandle hFoundColor = nullptr;
    sAICustomColor->CountCustomColors(&custColorCount);
    for (int i = 0; i < custColorCount; i++)
    {
        sAICustomColor->GetNthCustomColor(i, &hFoundColor);
        ai::UnicodeString currName;
        sAICustomColor->GetCustomColorName(hFoundColor, currName);
        string sName = currName.as_Platform();
        if (sName == color.Name())
        {
            outFoundColor->kind = kCustomColor;
            outFoundColor->c.c.color = hFoundColor;
            outFoundColor->c.c.tint = 0;
            return TRUE;
        }
    }
    return FALSE;
}


void BtSwatchList::AdjustAllColorsCallback(AIColor *color, void *userData, AIErr *result, AIBoolean *altered)
{
    AIReal tintPercent = BtColor(*color).Tint();
    
    BtColor btcolor(*color);
    
    if (btcolor.IsPantone() && tintPercent != 1)
    {
        bool found = FALSE;
        AIColor foundColor = GetColorDefinitionFromBook(btcolor.Name(), found);
        
        if (found)
        {
            //Check if a swatch already exists for this color and tint %
            foundColor.c.c.tint = tintPercent;

            *altered = TRUE;
            *color = foundColor;
            return;
        }
        else
        {
            if (btcolor.Kind() == kCustomColor && btcolor.CustomFlag() == 0)
            {
                btcolor.CustomFlag(kCustomSpotColor);
            }
        }
    }
    if (btcolor.IsBlack() && tintPercent < 1)
    {
        AICustomColorHandle hBlack = nullptr;
        sAICustomColor->GetCustomColorByName(ai::UnicodeString("Black"), &hBlack);
        color->kind = kCustomColor;
        color->c.c.tint = tintPercent;
        color->c.c.color = hBlack;
        
        AISwatchRef existingSwatch = Bt::BtColor(*color).CheckSwatchListForColor(); //TODO: was tolerance .0001
        
        if(existingSwatch != nullptr) {
            sAISwatchList->GetAIColor(existingSwatch, color);
        } else {
            CreateSwatch("", *color);
        }
        *altered = TRUE; return;
    }
    if (btcolor.IsWhite() || tintPercent == 1)
    {
        AICustomColorHandle hWhite = nullptr;
        sAICustomColor->GetCustomColorByName(ai::UnicodeString("White"), &hWhite);
        color->kind = kCustomColor;
        color->c.c.tint = 0;
        color->c.c.color = hWhite;
        *altered = TRUE; return;
    }
    
    
    *altered = FALSE; return;
}

std::vector<std::string> BtSwatchList::GetCurrentSwatchesAsStringVector()
{
    vector<string> swatches;
    int numSwatches = sAISwatchList->CountSwatches(nullptr);
    swatches.resize(numSwatches);
    
    for(int i=0; i<numSwatches; i++) {
        AISwatchRef currSwatch = sAISwatchList->GetNthSwatch(nullptr, i);
        
        ai::UnicodeString currSwatchName;
        sAISwatchList->GetSwatchName(currSwatch, currSwatchName);
        
        AIColor currSwatchColor;
        sAISwatchList->GetAIColor(currSwatch, &currSwatchColor);
        
        AIReal tint = BtColor(currSwatchColor).Tint();
        if (tint != 0) {
            int t = sAIRealMath->AIRealMultiple((1 - tint) * 100, 1, TRUE);
            currSwatchName.insert(0, ai::UnicodeString(std::to_string(t).append("% ")));
        }
        
        swatches[i] = currSwatchName.as_Platform();
    }
    
    return swatches;
}

string BtSwatchList::GetColorListAsXMLString()
{
    vector<string> swatches = GetCurrentSwatchesAsStringVector();
    
    //Format as XML string
    string xmlString;
    
    xmlString.append("<root>");
    for (auto it : swatches)
    {
        xmlString.append("<color>").append(it).append("</color>");
    }
    xmlString.append("</root>");
    
    return xmlString;
}

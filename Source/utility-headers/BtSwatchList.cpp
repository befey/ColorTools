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

//Constuctor
BtSwatchList::BtSwatchList()
{
    AICustomColorUnion c;
    c.f.cyan = 0;
    c.f.yellow = 0;
    c.f.magenta = 0;
    c.f.black = 1;
    BtColor BLACK(BLACK_COLOR_NAME, kCustomFourColor, c, 0);
    stdColorDefinitions[BLACK_COLOR_NAME] = BLACK;
    
    c.f.cyan = 0;
    c.f.yellow = 0;
    c.f.magenta = 0;
    c.f.black = 0;
    BtColor WHITE(WHITE_COLOR_NAME, kCustomFourColor, c, 0);
    stdColorDefinitions[WHITE_COLOR_NAME] = WHITE;

    c.f.cyan = 0;
    c.f.yellow = 0;
    c.f.magenta = 0;
    c.f.black = 1;
    BtColor MICRBLACK(MICR_BLACK_MAG_COLOR_NAME, kCustomFourColor, c, 0);
    stdColorDefinitions[MICR_BLACK_MAG_COLOR_NAME] = MICRBLACK;
}

//Behaviors
void BtSwatchList::CreateOrConvertToCustomColor(std::string colorName)
{
    CreateOrConvertToCustomColor(stdColorDefinitions[colorName]);
}

void BtSwatchList::CreateOrConvertToCustomColor(BtColor color)
{
    AICustomColor newCustomColorDefinition;
    newCustomColorDefinition.kind = color.GetKind();
    newCustomColorDefinition.c = color.GetCustomColorUnion();
    newCustomColorDefinition.flag = color.GetCustomColorFlags();
    
    AIColor newAiColorDefinition;
    
    if (ColorHasDefinitionAlready(color, &newAiColorDefinition) && newAiColorDefinition.kind == kCustomColor)
    {
        newAiColorDefinition.c.c.tint = 0;
        sAICustomColor->SetCustomColor(newAiColorDefinition.c.c.color, &newCustomColorDefinition);
    }
    else
    {
        AICustomColorHandle hCreatedCustomColor;
        sAICustomColor->NewCustomColor(&newCustomColorDefinition, ai::UnicodeString(color.GetName()), &hCreatedCustomColor);
        newAiColorDefinition.kind = kCustomColor;
        newAiColorDefinition.c.c.tint = 0;
        newAiColorDefinition.c.c.color = hCreatedCustomColor;
    }
    
    //TODO: Remove this so this function doesn't have side effects and just creates the color
    // we will just create swatches at the time we actually want them
    //CreateSwatch(color->GetName(), newAiColorDefinition);
    {
        AIColor dColor;
        AISwatchRef swatch = NULL;
        if (SwatchNameExists(color.GetName(), &dColor))
        {
            swatch = sAISwatchList->GetSwatchByName(NULL, ai::UnicodeString(color.GetName()));
        }
        else
        {
            swatch = sAISwatchList->InsertNthSwatch(NULL, -1);
        }
        sAISwatchList->SetAIColor(swatch, &newAiColorDefinition);
    }
}

void BtSwatchList::AdjustAllColors()
{
    VisitAIColorFlags controlFlags = kVisitColorsSolidOnly | kVisitGlobalObjectsOnceOnly;
    sAIPathStyle->AdjustObjectAIColors( NULL , AdjustAllColorsCallback , NULL , controlFlags , NULL );
    
    //sAIPathStyle->AdjustObjectAIColors( NULL , ConvertObjectsToGlobalCMYK , NULL , controlFlags , NULL );
    RemoveWhiteOverprint();
}

void BtSwatchList::RemoveUnusedColors() {
    int numSwatches = sAISwatchList->CountSwatches(NULL);
    
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
    
    size_t count;		sAIArtSet->CountArtSet( artSet, &count );
    bool used = FALSE;
    
    //Loop through all the swatches in the list
    for(int i=0; i<numSwatches; i++) {
        AISwatchRef currSwatch = sAISwatchList->GetNthSwatch(NULL, i);
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
        if( !currSwatchName.compare((ai::UnicodeString)NONE_COLOR_NAME) ||  //compare returns 0 if two strings are equal
           !currSwatchName.compare((ai::UnicodeString)REGISTRATION_COLOR_NAME) ||
           !currSwatchName.compare((ai::UnicodeString)BLACK_COLOR_NAME) ||
           !currSwatchName.compare((ai::UnicodeString)WHITE_COLOR_NAME) )
        {
            if (swatchTint == 0) {
                used = TRUE;
            }
        }
        
        //Loop through each art object to see if it uses the current swatch
        for ( int j=0 ; j< count ; j++ ) {
            if(used) {break;}
            AIArtHandle currArtObj = NULL;
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
        
        if (!used) { sAISwatchList->RemoveNthSwatch(NULL, i); numSwatches--; i--; }
    }
    //DISPOSE OF THE ART SET
    sAIArtSet->DisposeArtSet(&artSet); artSet = NULL;
    
    //Loop through the swatch groups and remove any empty ones
    count = sAISwatchGroup->CountSwatchGroups(NULL);
    for ( int i=0 ; i<count; i++) {
        AISwatchGroupRef currSwatchGroup = sAISwatchGroup->GetNthSwatchGroup(NULL, i);
        int currCount = sAISwatchGroup->CountSwatches(currSwatchGroup);
        if (!currCount) { sAISwatchGroup->RemoveSwatchGroup(NULL, currSwatchGroup, FALSE); i--; count--; }
    }
    
}


void BtSwatchList::CreateSwatch(std::string name, AIColor color)
{
    AISwatchRef swatch = sAISwatchList->GetSwatchByColor(NULL, &color);
    if (swatch == NULL)
    {
        swatch = sAISwatchList->InsertNthSwatch(NULL, -1);
    }
    sAISwatchList->SetAIColor(swatch, &color);
    if (!name.empty())
    {
        ASErr err = sAISwatchList->SetSwatchName(swatch, ai::UnicodeString(name));
        string error = GetIllustratorErrorCode(err);
    }
}

AISwatchRef BtSwatchList::GetSwatchByName(std::string name) const
{
    return sAISwatchList->GetSwatchByName(NULL, ai::UnicodeString(name));
}

bool BtSwatchList::ColorHasDefinitionAlready(BtColor color, AIColor* outFoundColor) const
{
    return SwatchNameExists(color.GetName(), outFoundColor) || CustomColorExists(color, outFoundColor);
}

bool BtSwatchList::SwatchNameExists(std::string name, AIColor* outFoundColor) const
{
    AISwatchRef hFoundSwatch = GetSwatchByName(name);
    if (hFoundSwatch != NULL)
    {
        sAISwatchList->GetAIColor(hFoundSwatch, outFoundColor);
        return TRUE;
    }
    return FALSE;
}

bool BtSwatchList::CustomColorExists(BtColor color, AIColor* outFoundColor) const
{
    int custColorCount = 0;
    AICustomColorHandle hFoundColor = NULL;
    sAICustomColor->CountCustomColors(&custColorCount);
    for (int i = 0; i < custColorCount; i++)
    {
        sAICustomColor->GetNthCustomColor(i, &hFoundColor);
        ai::UnicodeString currName;
        sAICustomColor->GetCustomColorName(hFoundColor, currName);
        string sName = currName.as_Platform();
        if (sName == color.GetName())
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
    AIReal tintPercent = GetTint(*color);
    
    if (ColorIsPantone(*color) && tintPercent != 1)
    {
        string colorName = GetColorName(*color);
        
        bool found = FALSE;
        AIColor foundColor = GetColorDefinitionFromBook(colorName, found);
        
        if (found)
        {
            //Check if a swatch already exists for this color and tint %
            colorName = GetColorName(foundColor);
            foundColor.c.c.tint = tintPercent;
            //AISwatchRef existingSwatch = checkSwatchListForColor(foundColor , .0001 );
            
            //if (existingSwatch == NULL)
            //{
            //    CreateSwatch(colorName, foundColor);
            //}
            *altered = TRUE;
            *color = foundColor;
            return;
        }
    }
    if (ColorIsBlack(*color) && tintPercent < 1)
    {
        AICustomColorHandle hBlack = NULL;
        sAICustomColor->GetCustomColorByName(ai::UnicodeString("Black"), &hBlack);
        color->kind = kCustomColor;
        color->c.c.tint = tintPercent;
        color->c.c.color = hBlack;
        
        AISwatchRef existingSwatch = checkSwatchListForColor( *color , .0001 );
        
        if(existingSwatch != NULL) {
            sAISwatchList->GetAIColor(existingSwatch, color);
        } else {
            CreateSwatch("", *color);
        }
        *altered = TRUE; return;
    }
    if (ColorIsWhite(*color) || tintPercent == 1)
    {
        AICustomColorHandle hWhite = NULL;
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
    int numSwatches = sAISwatchList->CountSwatches(NULL);
    swatches.resize(numSwatches);
    
    for(int i=0; i<numSwatches; i++) {
        AISwatchRef currSwatch = sAISwatchList->GetNthSwatch(NULL, i);
        
        ai::UnicodeString currSwatchName;
        sAISwatchList->GetSwatchName(currSwatch, currSwatchName);
        
        AIColor currSwatchColor;
        sAISwatchList->GetAIColor(currSwatch, &currSwatchColor);
        
        AIReal tint = GetTint(currSwatchColor);
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

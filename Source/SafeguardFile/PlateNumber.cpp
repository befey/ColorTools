//
//  PlateNumber.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/7/16.
//
//

#include "PlateNumber.h"
#include "ATEFuncs.h"
#include <regex>
#include "ColorFuncs.h"

using SafeguardFile::PlateNumber;
using SafeguardFile::ProductType;

PlateNumber::PlateNumber(string pNum)
{
    for (auto & c: pNum) c = toupper(c);
    plateNumber = pNum;
    
    isValidPlateNumber = TokenizePlateNumber();
    
    if (IsValid() && plantIndicator.length() == 0)
    {
        plantIndicator = productIndicator[0];
    }
}

Boolean PlateNumber::TokenizePlateNumber()
{
    std::regex r("(?:^(?:([a-z])(\\d{2}))?([a-z]{2})(\\d{3,6}).?\\S*)", std::regex::icase);
    
    std::smatch result;
    std::regex_search(plateNumber, result, r);
    
    /*There's two different plate number formats -- Y16SF000123 and SF00123 if we have a plate number in one of those formats we will get a result with matches:
        0: <the input plate number>
        1: <the plant indicator if the first type>
        2: <the year if the first type>
        3: <the product indicator>
        4: <the number>
        5: any trailing .BP, etc.
    */
    
    if (result.size() == 0) {
        return false;
    }
    
    else if (result[1].matched)
    {
        plantIndicator = result[1];
        year = result[2];
    }
    productIndicator = result[3];
    number = result[4];
    return true;
}

ProductType PlateNumber::GetProductType() const
{
    if (!IsValid())
    {
        return ProductType::INVAL;
    }
    
    if (productIndicator.at(0) == 'B' || productIndicator == "MP" || productIndicator == "SP")
    {
        return ProductType::BusinessStat;
    }
    
    if (productIndicator == "SF")
    {
        return ProductType::CutSheet;
    }
    
    if (productIndicator == "CF" || productIndicator == "SC" || productIndicator == "SS")
    {
        return ProductType::Continuous;
    }
    
    if (productIndicator == "EN")
    {
        return ProductType::Envelope;
    }
    
    if (productIndicator == "CS")
    {
        if (HasInnerTicks())
        {
            return ProductType::Snapset;
        }
        else
        {
            return ProductType::CutSheet;
        }
    }
    
    //OTHERS
    if (productIndicator == "AR" || productIndicator == "CK" || productIndicator == "CM" || productIndicator == "GC" || productIndicator == "LS" || productIndicator == "QC" || productIndicator == "RC" || productIndicator == "VP")
    {
        return ProductType::Snapset;
    }
    
    return ProductType::INVAL;
}

void PlateNumber::GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const
{
    AddTextToRange(plateNumber, targetRange);
}

Boolean PlateNumber::HasInnerTicks() const
{
    size_t count = 0;
    AIArtSet artSet = NULL;
    sAIArtSet->NewArtSet(&artSet);
    
    AIArtHandle currArtHandle = NULL;
    
    AIArtSpec specs[] = { { kPathArt , 0 , 0 } };
    
    sAIArtSet->MatchingArtSet( specs , 1, artSet );
    
    sAIArtSet->CountArtSet(artSet, &count);
    if(count == 0) {
        return false;
    }
    //Loop through the art set
    for (int i = 0 ; i < count ; i++) {
        
        sAIArtSet->IndexArtSet(artSet, i, &currArtHandle);
        AIBoolean closed;
        sAIPath->GetPathClosed(currArtHandle, &closed);
        if (!closed)
        {
            AIRealRect artBounds; sAIArt->GetArtBounds(currArtHandle, &artBounds);
            ai::ArtboardList artboardList;
            ai::ArtboardProperties props;
            sAIArtboard->GetArtboardList(artboardList);
            sAIArtboard->GetArtboardProperties(artboardList, 0, props);
            AIRealRect pageBounds;
            sAIArtboard->GetPosition(props, pageBounds);
            
            AIBoolean overlap = sAIRealMath->AIRealRectOverlap(&pageBounds, &artBounds);
            AIReal length;
            sAIPath->GetPathLength(currArtHandle, &length, NULL);
            if (overlap && sAIRealMath->EqualWithinTol(length, LENGTH_OF_INNER_TICK_PATH, 1))
            {
                vector<AIColor> colors = GetColorsFromArt(currArtHandle);
                for (auto color : colors)
                {
                    if (ColorIsRegistration(color))
                    {
                        return true;
                    }
                }
            }
        }
    }
    sAIArtSet->DisposeArtSet(&artSet);
    
    return false;
}

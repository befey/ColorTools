//
//  ColorList.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/22/16.
//
//

#include "ColorList.h"
#include "ColorFuncs.h"
#include "BtAteTextFeatures.h"
#include "SafeguardFileConstants.h"
#include "ArtTree.h"
#include "ColorEnumerator.hpp"
#include "DictionaryWriter.h"
#include "ColorListCommand.hpp"

ColorList::ColorList(AIRealRect area)
:
area(area)
{
    FillColorList();
    CleanUpColorListCommand(p_ColorList).Execute();
    Sort();
}

ColorList::ColorList(const ColorList& other)
:
p_ColorList(other.p_ColorList),
area(other.area)
{
    CleanUpColorListCommand(p_ColorList).Execute();
    Sort();
}

void ColorList::FillColorList()
{
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    CreateArtSetOfPrintingObjectsWithinRect(artSet, area);
    
    std::function<void(AIArtHandle)> func = std::bind(&ColorList::AddColorsOfArtToColorList, this, std::placeholders::_1);
    ProcessArtSet(artSet, func);
    sAIArtSet->DisposeArtSet(&artSet);
}

void ColorList::AddColorsOfArtToColorList(AIArtHandle art)
{
    AddColorsToList( ColorEnumerator(art).GetColorList() );
}

void ColorList::AddColorsToList(vector<AIColor> colors)
{
    p_ColorList.insert(std::end(p_ColorList), std::begin(colors), std::end(colors));
}

void ColorList::AddColorsToList(vector<BtColor> colors)
{
    p_ColorList.insert(std::end(p_ColorList), std::begin(colors), std::end(colors));
}

void ColorList::AddColorsToList(ColorList colors)
{
    p_ColorList.insert(p_ColorList.end(), colors.p_ColorList.begin(), colors.p_ColorList.end());
}

void ColorList::Sort()
{
    std::sort(p_ColorList.begin(), p_ColorList.end());
}

bool ColorList::HasCMYK() const
{
    for ( auto c : p_ColorList )
    {
        if (c.PrintsAsProcess())
        {
            return true;
        }
    }
    return false;
}

void ColorList::GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const
{
    if (p_ColorList.size() == 0)
    {
        BtColor(AIColor{.kind = kNoneColor}).AsTextRange(targetRange, maxWidth);
    }
    
    for ( auto c : p_ColorList )
    {
        c.AsTextRange(targetRange, maxWidth);
    }
}

void ColorList::SetColorMethod(string colorName, SafeguardFile::InkMethod method)
{
    for (auto& color : p_ColorList )
    {
        if (color.CompareName(colorName))
        {
            color.Method(method);
            break;
        }
    }
}

void ColorList::WriteColorListToArtDictionary(AIArtHandle art) const
{
    DictionaryWriter(art).AddVectorOfBtColorToDictionary(p_ColorList, COLORLIST_STORE);
}

void ColorList::ReadColorListFromArtDictionary(AIArtHandle art)
{
    p_ColorList.clear();
    DictionaryWriter(art).GetVectorOfBtColorFromIdentifier(p_ColorList, COLORLIST_STORE);
}

bool operator==(const ColorList& lhs, const ColorList& rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    
    vector<BtColor> lhsCopy = lhs.p_ColorList;
    sort(lhsCopy.begin(), lhsCopy.end());
    vector<BtColor> rhsCopy = rhs.p_ColorList;
    sort(rhsCopy.begin(), rhsCopy.end());

    return lhsCopy == rhsCopy;
}

//
//  ColorList.h
//  SafeguardTools
//
//  Created by T431962 on 6/22/16.
//
//

#ifndef __SafeguardTools__ColorList__
#define __SafeguardTools__ColorList__

#include "AIATETextUtil.h"
#include <vector>
#include "BtColor.h"
#include "ColorFuncs.h"
#include "SafeguardFileConstants.h"
#include "ICanBeTextRange.h"

extern AIATETextUtilSuite* sAIATETextUtil;

constexpr auto COLORLIST_STORE =            "__bt_colorlist_store__";

class ColorList : public ICanBeTextRange
{
public:
    ColorList(AIRealRect area);
    ColorList(vector<AIColor> colors) { AddColorsToList(colors); };
    ColorList(const ColorList& other);
    
    void AddColorsToList(vector<AIColor> colors);
    void AddColorsToList(vector<Bt::BtColor> colors);
    void AddColorsToList(ColorList colors);
    void Sort();
    
    bool HasCMYK() const;
    
    void SetColorMethod(string colorName, SafeguardFile::InkMethod method);
    
    void WriteColorListToArtDictionary(AIArtHandle art) const;
    void ReadColorListFromArtDictionary(AIArtHandle art);
    
    vector<Bt::BtColor>::iterator begin() { return p_ColorList.begin(); };
    vector<Bt::BtColor>::iterator end() { return p_ColorList.end(); };
    vector<Bt::BtColor>::const_iterator begin() const { return p_ColorList.begin(); };
    vector<Bt::BtColor>::const_iterator end() const { return p_ColorList.end(); };
    
    size_t size() const { return p_ColorList.size(); };
    
    Bt::BtColor& operator[](size_t index) { return p_ColorList[index]; }
    const Bt::BtColor& operator[](size_t index) const { return p_ColorList[index]; }
    
    friend bool operator==(const ColorList& lhs, const ColorList& rhs);
    friend bool operator!=(const ColorList& lhs, const ColorList& rhs) { return !operator==(lhs,rhs); };
private:
    vector<Bt::BtColor> p_ColorList;
    AIRealRect area{0,0,0,0};
    
    void FillColorList();
    void AddColorsOfArtToColorList(AIArtHandle art);
    
    void GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const override;
};

#endif /* defined(__SafeguardTools__ColorList__) */

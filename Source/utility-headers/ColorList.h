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

class ColorList : public ICanBeTextRange
{
public:
    ColorList(AIRealRect area);
    ColorList(vector<AIColor> colors) { AddColorsToList(colors); };
    
    void AddColorsToList(vector<AIColor> colors);
    void AddColorsToList(vector<BtColor> colors);
    void AddColorsToList(ColorList colors);
    void RemoveDuplicates();
    void RemoveNonPrintingColors();
    void Sort();
    
    bool HasCMYK();
    
    void SetColorMethod(string colorName, SafeguardFile::InkMethod method);
        
    vector<BtColor>::iterator begin() { return p_ColorList.begin(); };
    vector<BtColor>::iterator end() { return p_ColorList.end(); };
    vector<BtColor>::const_iterator begin() const { return p_ColorList.begin(); };
    vector<BtColor>::const_iterator end() const { return p_ColorList.end(); };
    
    size_t size() const { return p_ColorList.size(); };
    
    BtColor& operator[](size_t index) { return p_ColorList[index]; }
    const BtColor& operator[](size_t index) const { return p_ColorList[index]; }

private:
    vector<BtColor> p_ColorList;
    AIRealRect area;
    
    void FillColorList();
    void AddColorsOfArtToColorList(AIArtHandle art);
    
    void GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const override;
};

#endif /* defined(__SafeguardTools__ColorList__) */

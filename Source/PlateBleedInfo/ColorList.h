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

extern AIATETextUtilSuite* sAIATETextUtil;

namespace SafeguardFile
{
    class ColorList
    {
    public:
        void AddColorsToList(vector<AIColor> colors);
        void AddColorsToList(ColorList colors);
        void RemoveDuplicates();
        void RemoveNonPrintingColors();
        void Sort();
        
        void SetColorMethod(string colorName, SafeguardFile::InkMethod method);
        
        void GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const;
        const vector<BtColor> GetColorList() const { return p_ColorList; };
    private:
        vector<BtColor> p_ColorList;
        
        void AddColorToTextRange(const BtColor color, ATE::ITextRange& targetRange, AIReal maxWidth) const;
    };
}
#endif /* defined(__SafeguardTools__ColorList__) */

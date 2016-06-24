//
//  ColorList.h
//  SafeguardTools
//
//  Created by T431962 on 6/22/16.
//
//

#ifndef __SafeguardTools__ColorList__
#define __SafeguardTools__ColorList__

#include <vector>

namespace SafeguardFile
{
    class ColorList
    {
    public:
        void AddColorsToList(vector<AIColor> colors);
        void RemoveDuplicates();
        void RemoveNonPrintingColors();
        
        void GetAsTextRange(ATE::ITextRange& targetRange) const;
    
    private:
        vector<AIColor> p_ColorList;
        
        void AddColorToTextRange(const AIColor color, ATE::ITextRange& targetRange) const;
    };
}
#endif /* defined(__SafeguardTools__ColorList__) */

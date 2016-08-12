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
#include "BtColor.h"
#include "ColorFuncs.h"
#include "cereal/cereal.hpp"
#include "cereal/access.hpp"

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
        vector<BtColor> p_ColorList;
        
        void AddColorToTextRange(const BtColor color, ATE::ITextRange& targetRange) const;
        
        friend class cereal::access;
        template <class Archive>
        void serialize(Archive& ar)
        {
            int i = 0;
            for (auto color : p_ColorList)
            {
                ar( cereal::make_nvp(to_string(i++), color) );
            }
        }
    };
}
#endif /* defined(__SafeguardTools__ColorList__) */

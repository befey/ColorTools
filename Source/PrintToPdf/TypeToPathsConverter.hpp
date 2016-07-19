//
//  TypeToPathsConverter.hpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#ifndef TypeToPathsConverter_hpp
#define TypeToPathsConverter_hpp

#include "AIArtSet.h"
#include "BtArtboardRange.h"

extern AIArtSetSuite* sAIArtSet;

namespace PrintToPdf
{
    class TypeToPathsConverter
    {
    public:
        bool ConvertTypeToPaths(BtArtboardRange range) const;
    };
}

#endif /* TypeToPathsConverter_hpp */

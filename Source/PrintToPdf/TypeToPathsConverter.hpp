//
//  TypeToPathsConverter.hpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#ifndef TypeToPathsConverter_hpp
#define TypeToPathsConverter_hpp

#include "AILayer.h"
#include "BtArtboardRange.h"
#include "BtLayer.hpp"
#include <vector>

extern AILayerSuite* sAILayer;

extern AIArtSetSuite* sAIArtSet;

namespace PrintToPdf
{
    class TypeToPathsConverter
    {
    public:
        TypeToPathsConverter();
        
        bool ConvertTypeToPaths() const;
        
    private:
        vector<BtLayer> layerList;
    };
}

#endif /* TypeToPathsConverter_hpp */

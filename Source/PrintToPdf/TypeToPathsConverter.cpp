//
//  TypeToPathsConverter.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#include "TypeToPathsConverter.hpp"
#include "BtLayer.hpp"
#include "Plate.h"

using PrintToPdf::TypeToPathsConverter;

bool TypeToPathsConverter::ConvertTypeToPaths(BtArtboardRange range) const
{
    BtLayer layer(SafeguardFile::FOREGROUND_LAYER);
    if (layer)
    {
        layer.ConvertTextToPaths();
        return true;
    }
    else
    {
        
        return true;
    }
    
    return false;
}
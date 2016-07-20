//
//  TypeToPathsConverter.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#include "TypeToPathsConverter.hpp"
#include "Plate.h"

using PrintToPdf::TypeToPathsConverter;

TypeToPathsConverter::TypeToPathsConverter()
{
    ai::int32 count;
    sAILayer->CountLayers(&count);
    
    for (int i = 0; i < count; i++)
    {
        AILayerHandle layer;
        sAILayer->GetNthLayer(i, &layer);
        layerList.push_back(BtLayer(layer));
    }
}

bool TypeToPathsConverter::ConvertTypeToPaths() const
{
    for ( auto layer : layerList )
    {
        if (layer.Visible() && layer.Editable())
        {
            layer.ConvertTextToPaths();
        }
    }
    
    return true;
}
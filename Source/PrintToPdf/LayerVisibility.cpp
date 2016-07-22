//
//  LayerVisibility.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#include "LayerVisibility.hpp"
#include "Plate.h"

using PrintToPdf::LayerVisibility;
using PrintToPdf::BStatLayerVisibility;
using PrintToPdf::LaserLayerVisibility;
using PrintToPdf::BStatProofLayerVisibility;

LayerVisibility::LayerVisibility()
{
    ai::int32 count;
    sAILayer->CountLayers(&count);
    for (int i = 0; i < count; i++)
    {
        AILayerHandle layer;
        sAILayer->GetNthLayer(i, &layer);
        BtLayer btLayer(layer);
        layerList.insert(pair<string, BtLayer>(btLayer.Title(), btLayer));
    }
}

bool LayerVisibility::SetLayerVisibility()
{
    return CustomLayerVisibility();
}

bool BStatLayerVisibility::CustomLayerVisibility()
{
    for (auto& kv : layerList)
    {
        if (kv.first == SafeguardFile::KEYLINE_LAYER)
        {
            kv.second.Visible(true).Editable(true);
        }
        else if (kv.first == SafeguardFile::REG_LAYER)
        {
            kv.second.Visible(false).Editable(true);
        }
        else if (kv.first == SafeguardFile::SLUG_LAYER)
        {
            kv.second.Visible(true).Editable(false);
        }
        else if (kv.first == SafeguardFile::FOREGROUND_LAYER)
        {
            kv.second.Visible(true).Editable(true);
        }
        else if (kv.first == SafeguardFile::GUIDE_LAYER)
        {
            kv.second.Visible(false).Editable(true);
        }
        else if (kv.first == SafeguardFile::BACKGROUND_LAYER)
        {
            kv.second.Visible(false).Editable(true);
        }
        else
        {
            kv.second.Visible(false).Editable(true);
        }
    }
    
    return true;
}

bool LaserLayerVisibility::CustomLayerVisibility()
{
    for (auto& kv : layerList)
    {
        if (kv.first == SafeguardFile::FOREGROUND_LAYER)
        {
            kv.second.Visible(true).Editable(true);
        }
        else if (kv.first == SafeguardFile::GUIDE_LAYER)
        {
            kv.second.Visible(false).Editable(true);
        }
        else if (kv.first == SafeguardFile::BACKGROUND_LAYER)
        {
            kv.second.Visible(false).Editable(true);
        }
        else
        {
            kv.second.Visible(false).Editable(true);
        }
    }

    return true;
}

bool BStatProofLayerVisibility::CustomLayerVisibility() const
{
    for (const auto& kv : layerList)
    {
        if (kv.first == SafeguardFile::KEYLINE_LAYER)
        {
            kv.second.Visible(false).Editable(true);
        }
        else if (kv.first == SafeguardFile::REG_LAYER)
        {
            kv.second.Visible(true).Editable(true);
        }
        else if (kv.first == SafeguardFile::SLUG_LAYER)
        {
            kv.second.Visible(true).Editable(false);
        }
        else if (kv.first == SafeguardFile::FOREGROUND_LAYER)
        {
            kv.second.Visible(true).Editable(true);
        }
        else if (kv.first == SafeguardFile::GUIDE_LAYER)
        {
            kv.second.Visible(false).Editable(true);
        }
        else if (kv.first == SafeguardFile::BACKGROUND_LAYER)
        {
            kv.second.Visible(false).Editable(true);
        }
        else
        {
            kv.second.Visible(false).Editable(true);
        }
    }
    
    return true;
}
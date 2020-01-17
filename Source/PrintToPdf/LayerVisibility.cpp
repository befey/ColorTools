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
using PrintToPdf::NonStandardLayerVisibility;
using SafeguardFile::ProductType;

shared_ptr<LayerVisibility> LayerVisibility::GetLayerVisibility(SafeguardFile::ProductType productType, PdfPreset preset)
{
    //SETUP LAYER VISIBILITY
    if (productType == ProductType::BusinessStat)
    {
        if (preset == PdfPreset::MicrProof || preset == PdfPreset::Proof)
        {
            return shared_ptr<LayerVisibility> { make_shared<BStatProofLayerVisibility>() };
        }
        else
        {
            return shared_ptr<LayerVisibility> { make_shared<BStatLayerVisibility>() };
        }
    }
    else if (productType == ProductType::INVAL)
    {
        return shared_ptr<LayerVisibility> { make_shared<NonStandardLayerVisibility>() };
    }
    else
    {
        return shared_ptr<LayerVisibility> { make_shared<LaserLayerVisibility>() };
    }
}

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
            if (kv.second.Printed())
            {
                kv.second.Visible(true).Editable(true);
            }
            else
            {
                kv.second.Visible(false).Editable(true);
            }
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
            if (kv.second.Printed())
            {
                kv.second.Visible(true).Editable(true);
            }
            else
            {
                kv.second.Visible(false).Editable(true);
            }
        }
    }

    return true;
}

bool BStatProofLayerVisibility::CustomLayerVisibility()
{
    for (auto& kv : layerList)
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
            if (kv.second.Printed())
            {
                kv.second.Visible(true).Editable(true);
            }
            else
            {
                kv.second.Visible(false).Editable(true);
            }
        }
    }
    
    return true;
}

bool NonStandardLayerVisibility::CustomLayerVisibility()
{
    for (auto& kv : layerList)
    {
        if (kv.second.Printed())
        {
            kv.second.Visible(true).Editable(true);
        }
        else
        {
            kv.second.Visible(false).Editable(true);
        }
    }
    
    return true;
}

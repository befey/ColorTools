//
//  RequiredLayersCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/12/17.
//
//

#include "RequiredLayersCreator.hpp"
#include "BtLayer.hpp"
#include "SafeguardFileConstants.h"

using PlateBleedInfo::RequiredLayersCreator;

void RequiredLayersCreator::CreateRequiredLayers()
{
    ai::int32 count;
    sAILayer->CountLayers(&count);
    
    bool foundPrintingLayer = false;
    bool foundNonPrintingLayer = false;
    
    for (int i = 0; i < count; i++)
    {
        AILayerHandle layer;
        sAILayer->GetNthLayer(i, &layer);
        BtLayer btLayer(layer);
        if (!foundPrintingLayer && btLayer.Printed())
        {
            btLayer.Title(SafeguardFile::FOREGROUND_LAYER);
            foundPrintingLayer = true;
        }
        if (!foundNonPrintingLayer && !btLayer.Printed())
        {
            btLayer.Title(SafeguardFile::BACKGROUND_LAYER);
            foundNonPrintingLayer = true;
        }
        
        if (foundPrintingLayer && foundNonPrintingLayer)
        {
            break;
        }
    }
    
    if (!foundPrintingLayer)
    {
        BtLayer(SafeguardFile::FOREGROUND_LAYER).Printed(true).Visible(true).Editable(true);
    }
    if (!foundNonPrintingLayer)
    {
        BtLayer(SafeguardFile::BACKGROUND_LAYER).Printed(false).Visible(true).Editable(true);
    }
    
    BtLayer(SafeguardFile::FOREGROUND_LAYER).MoveToTop().MakeCurrent();
}

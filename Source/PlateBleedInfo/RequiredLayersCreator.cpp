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
    BtLayer foregroundLayer(SafeguardFile::FOREGROUND_LAYER, DoNotCreateLayer);
    if (!foregroundLayer.Null())
    {
        foundPrintingLayer = true;
    }
    
    bool foundNonPrintingLayer = false;
    BtLayer backgroundLayer(SafeguardFile::BACKGROUND_LAYER, DoNotCreateLayer);
    if (!backgroundLayer.Null())
    {
        foundNonPrintingLayer = true;
    }
    
    if (!foundPrintingLayer || !foundNonPrintingLayer)
    {
        for (int i = 0; i < count; i++)
        {
            AILayerHandle layer;
            sAILayer->GetNthLayer(i, &layer);
            BtLayer btLayer(layer);
            if (!foundPrintingLayer && btLayer.Printed())
            {
                foregroundLayer = btLayer;
                foundPrintingLayer = true;
            }
            if (!foundNonPrintingLayer && !btLayer.Printed())
            {
                backgroundLayer = btLayer;
                foundNonPrintingLayer = true;
            }
            
            if (foundPrintingLayer && foundNonPrintingLayer)
            {
                break;
            }
        }
    }
    
    if (!foundPrintingLayer)
    {
        foregroundLayer = BtLayer(SafeguardFile::FOREGROUND_LAYER);
    }
    if (!foundNonPrintingLayer)
    {
        backgroundLayer = BtLayer(SafeguardFile::BACKGROUND_LAYER);
    }
    
    foregroundLayer.Printed(true).Visible(true).Editable(true).MoveToTop().MakeCurrent();
    backgroundLayer.Printed(false).Visible(true).Editable(true);
}

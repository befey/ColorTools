//
//  BtLayer.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#include "BtLayer.hpp"


BtLayer::BtLayer(AILayerHandle layer) : layerHandle(layer) {}

void BtLayer::DeleteLayer() const
{
    sAILayer->DeleteLayer(layerHandle);
}

string BtLayer::Title() const
{
    ai::UnicodeString usTitle;
    sAILayer->GetLayerTitle(layerHandle, usTitle);
    return usTitle.as_Platform();
}
const BtLayer& BtLayer::Title(string title) const
{
    sAILayer->SetLayerTitle(layerHandle, ai::UnicodeString(title));
    return *this;
}

bool BtLayer::Visible() const
{
    AIBoolean vis;
    sAILayer->GetLayerVisible(layerHandle, &vis);
    return vis;
}
const BtLayer& BtLayer::Visible(bool visible) const
{
    sAILayer->SetLayerVisible(layerHandle, visible);
    return *this;
}

bool BtLayer::Editable() const
{
    AIBoolean edit;
    sAILayer->GetLayerEditable(layerHandle, &edit);
    return edit;
}
const BtLayer& BtLayer::Editable(bool editable) const
{
    sAILayer->SetLayerEditable(layerHandle, editable);
    return *this;
}

bool BtLayer::Printed() const
{
    AIBoolean print;
    sAILayer->GetLayerPrinted(layerHandle, &print);
    return print;
}
const BtLayer& BtLayer::Printed(bool printed) const
{
    sAILayer->SetLayerPrinted(layerHandle, printed);
    return *this;
}

void BtLayer::DeselectArt() const
{
    sAILayer->DeselectArtOnLayer(layerHandle);
}
void BtLayer::SelectArt() const
{
    sAILayer->SelectArtOnLayer(layerHandle);
}


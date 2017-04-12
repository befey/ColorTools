//
//  BtLayer.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#include "BtLayer.hpp"
#include "BtArtHandle.hpp"
#include "ArtTree.h"
#include "Plate.h"
#include "GetIllustratorErrorCode.h"


BtLayer::BtLayer(AILayerHandle layer) : layerHandle(layer) {}
BtLayer::BtLayer(string layerName)
{
    sAILayer->GetLayerByTitle(&layerHandle, ai::UnicodeString(layerName));
    
    if (layerHandle == nullptr)
    {
        sAILayer->InsertLayer(nullptr, kPlaceAboveAll, &layerHandle);
        Title(layerName);
    }
}

BtLayer::BtLayer(int layerIndex)
{
    sAILayer->GetNthLayer(layerIndex, &layerHandle);
}

void BtLayer::DeleteLayer()
{
    if (layerHandle)
    {
        sAILayer->DeleteLayer(layerHandle);
        layerHandle = nullptr;
    }
}

string BtLayer::Title() const
{
    ai::UnicodeString usTitle;
    if (layerHandle)
    {
        sAILayer->GetLayerTitle(layerHandle, usTitle);
    }
    return usTitle.as_Platform();
}
BtLayer& BtLayer::Title(string title)
{
    if (layerHandle)
    {
        sAILayer->SetLayerTitle(layerHandle, ai::UnicodeString(title));
    }
    return *this;
}

bool BtLayer::Visible() const
{
    AIBoolean vis = false;
    if (layerHandle)
    {
        sAILayer->GetLayerVisible(layerHandle, &vis);
    }
    return vis;
}
BtLayer& BtLayer::Visible(bool visible)
{
    if (layerHandle)
    {
        sAILayer->SetLayerVisible(layerHandle, visible);
    }
    return *this;
}

bool BtLayer::Editable() const
{
    AIBoolean edit = false;
    if (layerHandle)
    {
        sAILayer->GetLayerEditable(layerHandle, &edit);
    }
    return edit;
}
BtLayer& BtLayer::Editable(bool editable)
{
    if (layerHandle)
    {
        sAILayer->SetLayerEditable(layerHandle, editable);
    }
    return *this;
}

bool BtLayer::Printed() const
{
    AIBoolean print = false;
    if (layerHandle)
    {
        sAILayer->GetLayerPrinted(layerHandle, &print);
    }
    return print;
}
BtLayer& BtLayer::Printed(bool printed)
{
    if (layerHandle)
    {
        sAILayer->SetLayerPrinted(layerHandle, printed);
    }
    return *this;
}

void BtLayer::DeselectArt() const
{
    if (layerHandle)
    {
        sAILayer->DeselectArtOnLayer(layerHandle);
    }
}
void BtLayer::SelectArt() const
{
    if (layerHandle)
    {
        sAILayer->SelectArtOnLayer(layerHandle);
    }
}

long BtLayer::GetArtSet(AIArtSet const targetSet) const
{
    if (!layerHandle)
    {
        return 0;
    }
    size_t count = 0;
    
    sAIArtSet->LayerArtSet(layerHandle, targetSet);
    
    sAIArtSet->CountArtSet(targetSet, &count);
    
    return count;
}

void BtLayer::ConvertTextToPaths() const
{
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    
    GetArtSet(artSet);
    
    ProcessArtSet(artSet, [](AIArtHandle handle)
                  {
                      BtArtHandle btHandle(handle);
                      
                      if (btHandle.ArtType() == kTextFrameArt)
                      {
                          if (!btHandle.Hidden())
                          {
                              btHandle.Locked(false);
                              
                              //Convert the type to paths
                              AIArtHandle tempNewPaths;
                              sAITextFrame->CreateOutline(handle, &tempNewPaths);
                              btHandle.Dispose();
                          }
                      }
                  });
    
    sAIArtSet->DisposeArtSet(&artSet);
}

void BtLayer::PutArtAtTopOfLayer(AIArtHandle art)
{
    AIArtHandle layerGroup = GetLayerGroupArt();

    MakeEditable();
    
    BtArtHandle btHandle(art);

    btHandle.MakeEditable();
    
    btHandle.PutInGroup(layerGroup);
    
    ResetEditable();
    
    btHandle.ResetEditable();
}

AIArtHandle BtLayer::GetLayerGroupArt() const
{
    if (layerHandle == nullptr)
    {
        return nullptr;
    }
    AIArtHandle layerGroup;
    sAIArt->GetFirstArtOfLayer(layerHandle, &layerGroup);
    return layerGroup;
}

BtLayer& BtLayer::MoveToTop()
{
    if (layerHandle)
    {
        BtArtHandle layerGroup(GetLayerGroupArt());
        BtArtHandle topLayerGroup(BtLayer(int(0)).GetLayerGroupArt());
    
        sAIArt->ReorderArt(layerGroup, kPlaceAbove, topLayerGroup);
    }

    return *this;
}

BtLayer& BtLayer::MakeEditable()
{
    storedEditable = Editable();
    Editable(true);
    storedVisible = Visible();
    Visible(true);
    
    return *this;
}

BtLayer& BtLayer::ResetEditable()
{
    Editable(storedEditable);
    Visible(storedVisible);
    
    return *this;
}

BtLayer& BtLayer::MakeCurrent()
{
    if (layerHandle)
    {
        sAILayer->SetCurrentLayer(layerHandle);
    }
    
    return *this;
}

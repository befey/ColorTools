//
//  BtLayer.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#include "BtLayer.hpp"
#include "ArtTree.h"
#include "Plate.h"


BtLayer::BtLayer(AILayerHandle layer) : layerHandle(layer) {}
BtLayer::BtLayer(string layerName)
{
    sAILayer->GetLayerByTitle(&layerHandle, ai::UnicodeString(layerName));
}

void BtLayer::DeleteLayer()
{
    if (layerHandle)
    {
        sAILayer->DeleteLayer(layerHandle);
        layerHandle = NULL;
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
    AIBoolean vis;
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
    AIBoolean edit;
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
    AIBoolean print;
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
                      short type;
                      sAIArt->GetArtType(handle, &type);
                      
                      if (type == kTextFrameArt)
                      {
                          int attr = 0;
                          //Check if the art is visible
                          sAIArt->GetArtUserAttr(handle, kArtHidden, &attr);
                          if (!(attr & kArtHidden))
                          {
                              //Check if the art is editable
                              attr = 0;
                              sAIArt->GetArtUserAttr(handle, kArtLocked, &attr);
                              if (attr & kArtLocked)
                              {
                                  sAIArt->SetArtUserAttr(handle, kArtLocked, 0);
                              }
                              
                              //Convert the type to paths
                              AIArtHandle tempNewPaths;
                              sAITextFrame->CreateOutline(handle, &tempNewPaths);
                              sAIArt->DisposeArt(handle);
                          }
                      }
                  });
    
}

void BtLayer::PutArtAtTopOfLayer(AIArtHandle art)
{
    AIArtHandle layerGroup = GetLayerGroupArt();

    MakeLayerEditableAndStorePreviousState();
    
    int attr = 0;
    
    //Check if the art itself is editable
    sAIArt->GetArtUserAttr(art, kArtLocked | kArtHidden, &attr);
    if ((attr & kArtLocked) || (attr & kArtHidden))
    {
        sAIArt->SetArtUserAttr(art, kArtLocked | kArtHidden, 0);
    }
    
    sAIArt->ReorderArt(art, kPlaceInsideOnTop, layerGroup);
    
    ApplyStoredAttributes();
    
    sAIArt->SetArtUserAttr(art, kArtLocked | kArtHidden, attr);
}

AIArtHandle BtLayer::GetLayerGroupArt() const
{
    AIArtHandle layerGroup;
    sAIArt->GetFirstArtOfLayer(layerHandle, &layerGroup);
    return layerGroup;
}

void BtLayer::MakeLayerEditableAndStorePreviousState()
{
    //Unlock and Unhide the layer
    if (!Editable())
    {
        Editable(TRUE);
        editableWasFalse = true;
    }
    if (!Visible())
    {
        Visible(TRUE);
        visibleWasFalse = true;
    }
}

void BtLayer::ApplyStoredAttributes()
{
    //Set the layer and art attributes back the way they were
    if (editableWasFalse)
    {
        Editable(FALSE);
    }
    if (visibleWasFalse)
    {
        Visible(FALSE);
    }
}
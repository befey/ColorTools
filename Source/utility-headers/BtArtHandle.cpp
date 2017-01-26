//
//  BtArtHandle.cpp
//  SafeguardTools
//
//  Created by T431962 on 1/26/17.
//
//

#include "BtArtHandle.hpp"
#include "BtLayer.hpp"

BtArtHandle::BtArtHandle(AIArtHandle artHandle)
:
artHandle(artHandle)
{}

bool BtArtHandle::GetAttribute(ai::int32 whichAttr) const
{
    ai::int32 attr;
    sAIArt->GetArtUserAttr(artHandle, whichAttr, &attr);
    return attr & whichAttr;
}

bool BtArtHandle::SetAttribute(ai::int32 whichAttr, bool state)
{
    ai::int32 attr = 0; //TURN OFF
    if (state)
    {
        attr = whichAttr; //TURN ON
    }
    ASErr err = sAIArt->SetArtUserAttr(artHandle, whichAttr, attr);
    
    if (err)
    {
        return false;
    }
    return true;
}

AILayerHandle BtArtHandle::Layer() const
{
    AILayerHandle layer;
    if ( kNoErr == sAIArt->GetLayerOfArt(artHandle, &layer) )
    {
        return layer;
    }
    return nullptr;
}

BtArtHandle& BtArtHandle::Layer(AILayerHandle newVal)
{
    if (newVal)
    {
        BtLayer(newVal).PutArtAtTopOfLayer(artHandle);
    }
    return *this;
}

short BtArtHandle::ArtType() const
{
    short type;
    sAIArt->GetArtType(artHandle, &type);
    return type;
}

BtArtHandle BtArtHandle::Parent() const
{
    AIArtHandle parent;
    sAIArt->GetArtParent(artHandle, &parent);
    return BtArtHandle(parent);
}

BtArtHandle BtArtHandle::FirstChild() const
{
    AIArtHandle child;
    sAIArt->GetArtFirstChild(artHandle, &child);
    return BtArtHandle(child);
}

BtArtHandle BtArtHandle::LastChild() const
{
    AIArtHandle child;
    sAIArt->GetArtLastChild(artHandle, &child);
    return BtArtHandle(child);
}

BtArtHandle BtArtHandle::Sibling() const
{
    AIArtHandle sibling;
    sAIArt->GetArtSibling(artHandle, &sibling);
    return BtArtHandle(sibling);
}

BtArtHandle BtArtHandle::PriorSibling() const
{
    AIArtHandle sibling;
    sAIArt->GetArtPriorSibling(artHandle, &sibling);
    return BtArtHandle(sibling);
}

bool BtArtHandle::LayerGroup() const
{
    ASBoolean isLayerGroup;
    sAIArt->IsArtLayerGroup(artHandle, &isLayerGroup);
    return isLayerGroup;
}

bool BtArtHandle::IsArtClipping() const
{
    sAIArt->IsArtClipping(artHandle);
}

AIRealRect BtArtHandle::Bounds() const
{
    AIRealRect bounds;
    sAIArt->GetArtBounds(artHandle, &bounds);
    return bounds;
}

bool BtArtHandle::ValidArt() const
{
    return sAIArt->ValidArt(artHandle, true);
}

bool BtArtHandle::Selected() const
{
    return GetAttribute(kArtSelected);
}

bool BtArtHandle::Selected(bool state)
{
    return SetAttribute(kArtSelected, state);
}

bool BtArtHandle::Locked() const
{
    return GetAttribute(kArtLocked);
}

bool BtArtHandle::Locked(bool state)
{
    return SetAttribute(kArtLocked, state);
}

bool BtArtHandle::Hidden() const
{
    return GetAttribute(kArtHidden);
}

bool BtArtHandle::Hidden(bool state)
{
    return SetAttribute(kArtHidden, state);
}

bool BtArtHandle::FullySelected() const
{
    return GetAttribute(kArtFullySelected);
}

bool BtArtHandle::FullySelected(bool state)
{
    return SetAttribute(kArtFullySelected, state);
}

bool BtArtHandle::ClipMask() const
{
    return GetAttribute(kArtIsClipMask);
}

bool BtArtHandle::ClipMask(bool state)
{
    return SetAttribute(kArtIsClipMask, state);
}

bool BtArtHandle::PartOfCompound() const
{
    return GetAttribute(kArtPartOfCompound);
}

size_t BtArtHandle::TimeStamp(AIArtTimeStampOptions options) const
{
    size_t timeStamp;
    sAIArt->GetArtTimeStamp(artHandle, options, &timeStamp);
    return timeStamp;
}

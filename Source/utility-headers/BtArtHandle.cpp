//
//  BtArtHandle.cpp
//  SafeguardTools
//
//  Created by T431962 on 1/26/17.
//
//

#include "BtArtHandle.hpp"
#include "BtLayer.hpp"
#include "GetIllustratorErrorCode.h"
#include "ArtTree.h"

BtArtHandle::BtArtHandle(AIArtHandle artHandle)
:
artHandle(artHandle)
{}

bool operator==(const BtArtHandle& lhs, const BtArtHandle& rhs)
{
    return lhs.artHandle == rhs.artHandle;
}

bool BtArtHandle::GetAttribute(ai::int32 whichAttr) const
{
    ai::int32 attr;
    if (artHandle)
    {
        sAIArt->GetArtUserAttr(artHandle, whichAttr, &attr);
        return attr & whichAttr;
    }
    return false;
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

void BtArtHandle::Dispose()
{
    sAIArt->DisposeArt(artHandle);
    artHandle = nullptr;
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

BtArtHandle& BtArtHandle::PutInGroup(AIArtHandle theGroup)
{
    BtArtHandle btTheGroup(theGroup);
    if (btTheGroup.ArtType() != kGroupArt)
    {
        return *this;
    }
    
    BtLayer layer(Layer());
    if (layer.Null())
    {
        return *this;
    }

    //Set the layer editable and visible, set flags so we can set it back the way it was
    bool eflag = false;
    bool vflag = false;
    if (!layer.Editable()) { layer.Editable(true); eflag = true; }
    if (!layer.Visible()) { layer.Visible(true); vflag = true; }
    
    //Check if the art itself is editable
    bool alflag = false;
    bool ahflag = false;
    if (Locked()) { Locked(false); alflag = true; }
    if (Hidden()) { Hidden(false); ahflag = true; }
    
    //Check if the group is editable
    bool glflag = false;
    bool ghflag = false;
    if (btTheGroup.Locked()) { btTheGroup.Locked(false); glflag = true; }
    if (btTheGroup.Hidden()) { btTheGroup.Hidden(false); ghflag = true; }
    
    //Put it in the group
    sAIArt->ReorderArt(artHandle, kPlaceInsideOnTop, btTheGroup);
    //Set the layer and art attributes back the way they were
    if(eflag) { layer.Editable(false); }
    if(vflag) { layer.Visible(false); }
    if(alflag) { Locked(true); }
    if(ahflag) { Hidden(true); }
    if(glflag) { btTheGroup.Locked(true); }
    if(ghflag) { btTheGroup.Hidden(true); }
    
    return *this;
}

bool BtArtHandle::IsInArtSet(AIArtSet theSet) const
{
    size_t count;
    sAIArtSet->CountArtSet( theSet, &count );
    
    for ( int i=0 ; i < count ; i++ )
    {
        AIArtHandle currArtHandle = nullptr;
        sAIArtSet->IndexArtSet( theSet, i, &currArtHandle );
        if (currArtHandle == artHandle)
        {
            return true;
        }
    }
    return false;
}

short BtArtHandle::ArtType() const
{
    short type;
    if ( kNoErr == sAIArt->GetArtType(artHandle, &type) )
    {
        return type;
    }
    return kUnknownArt;
}

BtArtHandle BtArtHandle::Parent() const
{
    AIArtHandle parent = nullptr;
    sAIArt->GetArtParent(artHandle, &parent);
    return BtArtHandle(parent);
}

BtArtHandle BtArtHandle::FirstChild() const
{
    AIArtHandle child = nullptr;
    sAIArt->GetArtFirstChild(artHandle, &child);
    return BtArtHandle(child);
}

BtArtHandle BtArtHandle::LastChild() const
{
    AIArtHandle child = nullptr;
    sAIArt->GetArtLastChild(artHandle, &child);
    return BtArtHandle(child);
}

BtArtHandle BtArtHandle::Sibling() const
{
    AIArtHandle sibling = nullptr;
    sAIArt->GetArtSibling(artHandle, &sibling);
    return BtArtHandle(sibling);
}

BtArtHandle BtArtHandle::PriorSibling() const
{
    AIArtHandle sibling = nullptr;
    sAIArt->GetArtPriorSibling(artHandle, &sibling);
    return BtArtHandle(sibling);
}

bool BtArtHandle::LayerGroup() const
{
    ASBoolean isLayerGroup;
    sAIArt->IsArtLayerGroup(artHandle, &isLayerGroup);
    return isLayerGroup;
}

bool BtArtHandle::IsClippingGroup() const
{
    if (ArtType() == kGroupArt)
    {
        AIBoolean clipped;
        sAIGroup->GetGroupClipped(artHandle, &clipped);
        return clipped;
    }
    return false;
}

AIRealRect BtArtHandle::Bounds() const
{
    if (!Null())
    {
        AIRealRect bounds;
        sAIArt->GetArtBounds(artHandle, &bounds);
        return bounds;
    }
    return {0,0,0,0};
}

bool BtArtHandle::ClippedBounds(AIRealRect& result) const
{
    bool boundsValid = false;
    BtArtHandle enclosingClipArt = GetEnclosingClipArt();
    
    while (!enclosingClipArt.Null())
    {
        AIRealRect clipRect = enclosingClipArt.Bounds();
        AIRealRect thisRect;
        if (boundsValid)
        {
            thisRect = result;
        }
        else
        {
            thisRect = Bounds();
        }
        
        boundsValid = sAIRealMath->AIRealRectIntersect(&clipRect, &thisRect, &result);
        if (boundsValid)
        {
            enclosingClipArt = enclosingClipArt.Parent().GetEnclosingClipArt();
        }
        else
        {
            return boundsValid;
        }
    }
    
    return boundsValid;
}

AIRealPoint BtArtHandle::Center() const
{
    return GetCenterOfRect(Bounds());
}

BtArtHandle BtArtHandle::GetEnclosingClipArt() const
{
    BtArtHandle layerGroup(BtLayer(Layer()).GetLayerGroupArt());
    BtArtHandle parent = Parent();
    
    if (*this != layerGroup && parent != layerGroup)
    {
        if (parent.IsClippingGroup())
        {
            BtArtHandle sibling = parent.FirstChild();
            do
            {
                if (sibling.ClipMask())
                {
                    return sibling;
                }
                sibling = sibling.Sibling();
            } while (!sibling.Null());
        }
        
        return parent.GetEnclosingClipArt();
    }
    
    return BtArtHandle();
}

bool BtArtHandle::OverlapsRect(AIRealRect rect) const
{
    AIRealRect artRect;
    if (!ClippedBounds(artRect))
    {
        artRect = Bounds();
    }
    return sAIRealMath->AIRealRectOverlap(&artRect, &rect);
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
    if (Hidden()) Hidden(false);
    if (Locked()) Locked(false);
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

void BtArtHandle::MakeEditable()
{
    storedLocked = Locked();
    Locked(false);
    storedHidden = Hidden();
    Hidden(false);
}

void BtArtHandle::ResetEditable()
{
    Locked(storedLocked);
    Hidden(storedHidden);
}

BtArtHandle& BtArtHandle::PathStyle(AIPathStyle newVal)
{
    if (!Null())
    {
        sAIPathStyle->SetPathStyle(artHandle, &newVal);
    }
    return *this;
}

AIPathStyle BtArtHandle::PathStyle() const
{
    AIPathStyle val;
    if (!Null())
    {
        sAIPathStyle->GetPathStyle(artHandle, &val);
    }
    return val;
}

BtArtHandle& BtArtHandle::Stroke(bool newVal)
{
    AIPathStyle style = PathStyle();
    style.strokePaint = newVal;
    PathStyle(style);
    return *this;
}

bool BtArtHandle::Stroke() const
{
    return PathStyle().strokePaint;
}

BtArtHandle& BtArtHandle::StrokeColor(BtColor newVal)
{
    AIStrokeStyle style = StrokeStyle();
    style.color = newVal;
    StrokeStyle(style);
    return *this;
}

BtColor BtArtHandle::StrokeColor() const
{
    return StrokeStyle().color;
}

BtArtHandle& BtArtHandle::StrokeWeight(AIReal newVal)
{
    AIStrokeStyle style = StrokeStyle();
    style.width = newVal;
    StrokeStyle(style);
    return *this;
}

AIReal BtArtHandle::StrokeWeight() const
{
    return StrokeStyle().width;
}

BtArtHandle& BtArtHandle::OverprintStroke(bool newVal)
{
    AIStrokeStyle style = StrokeStyle();
    style.overprint = newVal;
    StrokeStyle(style);
    return *this;
}

bool BtArtHandle::OverprintStroke() const
{
    return StrokeStyle().overprint;
}

BtArtHandle& BtArtHandle::StrokeStyle(AIStrokeStyle newVal)
{
    AIPathStyle style = PathStyle();
    style.stroke = newVal;
    PathStyle(style);
    return *this;
}

AIStrokeStyle BtArtHandle::StrokeStyle() const
{
    return PathStyle().stroke;
}

BtArtHandle& BtArtHandle::Fill(bool newVal)
{
    AIPathStyle style = PathStyle();
    style.fillPaint = newVal;
    PathStyle(style);
    return *this;
}

bool BtArtHandle::Fill() const
{
    return PathStyle().fillPaint;
}

BtArtHandle& BtArtHandle::FillColor(BtColor newVal)
{
    AIFillStyle style = FillStyle();
    style.color = newVal;
    FillStyle(style);
    return *this;
}

BtColor BtArtHandle::FillColor() const
{
    return FillStyle().color;
}

BtArtHandle& BtArtHandle::OverprintFill(bool newVal)
{
    AIFillStyle style = FillStyle();
    style.overprint = newVal;
    FillStyle(style);
    return *this;
}

bool BtArtHandle::OverprintFill() const
{
    return FillStyle().overprint;
}

BtArtHandle& BtArtHandle::FillStyle(AIFillStyle newVal)
{
    AIPathStyle style = PathStyle();
    style.fill = newVal;
    PathStyle(style);
    return *this;
}

AIFillStyle BtArtHandle::FillStyle() const
{
    return PathStyle().fill;
}


size_t BtArtHandle::TimeStamp(AIArtTimeStampOptions options) const
{
    size_t timeStamp;
    if ( kNoErr == sAIArt->GetArtTimeStamp(artHandle, options, &timeStamp) )
    {
        return timeStamp;
    }
    return 0;
}

ATE::ITextRange BtArtHandle::ITextRange() const
{
    if (ArtType() == kTextFrameArt)
    {
        ATE::TextRangeRef currRangeRef = nullptr;
        sAITextFrame->GetATETextRange(artHandle, &currRangeRef);
        return ATE::ITextRange(currRangeRef);
    }
    return ITextRange();
}

void BtArtHandle::VisitEachArtInTree(void (*visitFunc)(BtArtHandle)) const
{
    BtArtHandle currArt = *this;
    while (!currArt.Null())
    {
        visitFunc(currArt);
        
        if ( !currArt.FirstChild().Null() )
        {
            currArt = currArt.FirstChild();
        }
        else
        {
            while (currArt.Sibling().Null() && currArt.artHandle != this->artHandle)
            {
                currArt = currArt.Parent();
            }
            
            if (currArt.artHandle != this->artHandle)
            {
                currArt = currArt.Sibling();
            }
            else
            {
                break;
            }
        }
    }
}

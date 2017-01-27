//
//  BtArtHandle.hpp
//  SafeguardTools
//
//  Created by T431962 on 1/26/17.
//
//

#ifndef BtArtHandle_hpp
#define BtArtHandle_hpp

#include "AIArt.h"
#include "AIRealMath.h"
#include "AIGroup.h"

extern AIArtSuite* sAIArt;
extern AIRealMathSuite* sAIRealMath;
extern AIGroupSuite* sAIGroup;

class BtArtHandle
{
public:
    BtArtHandle(){};
    BtArtHandle(AIArtHandle artHandle);
    
    bool Null() const { return !artHandle; };
    
    AILayerHandle Layer() const;
    BtArtHandle& Layer(AILayerHandle newVal);
    
    short ArtType() const;
    
    BtArtHandle Parent() const;
    BtArtHandle FirstChild() const;
    BtArtHandle LastChild() const;
    BtArtHandle Sibling() const;
    BtArtHandle PriorSibling() const;
    bool LayerGroup() const;
    
    bool IsClippingGroup() const;
    
    AIRealRect Bounds() const;
    bool ClippedBounds(AIRealRect& result) const;
    bool OverlapsRect(AIRealRect rect) const;
    
    bool ValidArt() const;
    
    bool Selected() const;
    bool Selected(bool state);
    bool Locked() const;
    bool Locked(bool state);
    bool Hidden() const;
    bool Hidden(bool state);
    bool FullySelected() const;
    bool FullySelected(bool state);
    bool ClipMask() const;
    bool ClipMask(bool state);
    bool PartOfCompound() const;
    
    size_t TimeStamp(AIArtTimeStampOptions options = kAITimeStampOfArt) const;
    
    inline operator const AIArtHandle(void) const { return artHandle; }
    inline operator AIArtHandle*(void) { return &artHandle; }
    inline operator bool(void) const { return Null(); }
    
    friend bool operator==(const BtArtHandle& lhs, const BtArtHandle& rhs);
    friend bool operator!=(const BtArtHandle& lhs, const BtArtHandle& rhs) { return !operator==(lhs,rhs); };
private:
    AIArtHandle artHandle = nullptr;
    
    bool GetAttribute(ai::int32 whichAttr) const;
    bool SetAttribute(ai::int32 whichAttr, bool state);
    
    BtArtHandle GetEnclosingClipArt() const;
};

#endif /* BtArtHandle_hpp */

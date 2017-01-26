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

extern AIArtSuite* sAIArt;

class BtArtHandle
{
public:
    BtArtHandle(AIArtHandle artHandle);
    
    bool Null() const { return artHandle; };
    
    AILayerHandle Layer() const;
    BtArtHandle& Layer(AILayerHandle newVal);
    
    short ArtType() const;
    
    BtArtHandle Parent() const;
    BtArtHandle FirstChild() const;
    BtArtHandle LastChild() const;
    BtArtHandle Sibling() const;
    BtArtHandle PriorSibling() const;
    bool LayerGroup() const;
    
    bool IsArtClipping() const;
    
    AIRealRect Bounds() const;
    
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
    
    inline operator AIArtHandle(void) const { return artHandle; }
    inline operator bool(void) const { return Null(); }
private:
    AIArtHandle artHandle;
    
    bool GetAttribute(ai::int32 whichAttr) const;
    bool SetAttribute(ai::int32 whichAttr, bool state);
};

#endif /* BtArtHandle_hpp */

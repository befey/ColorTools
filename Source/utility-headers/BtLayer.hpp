//
//  BtLayer.hpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#ifndef BtLayer_hpp
#define BtLayer_hpp

#include "AILayer.h"
#include "AIArtSet.h"

extern AILayerSuite* sAILayer;
extern AIArtSetSuite* sAIArtSet;

class BtLayer
{
public:
    BtLayer(AILayerHandle layer);
    BtLayer(string layerName);
    inline operator bool(void) const { return (layerHandle) ? true : false; };
    
    void DeleteLayer();
    
    string Title() const;
    const BtLayer& Title(string title) const;
    
    bool Visible() const;
    const BtLayer& Visible(bool visible) const;
    
    bool Editable() const;
    const BtLayer& Editable(bool editable) const;
    
    bool Printed() const;
    const BtLayer& Printed(bool printed) const;
    
    void DeselectArt() const;
    void SelectArt() const;
    
    long GetArtSet(AIArtSet const targetSet) const;
    void ConvertTextToPaths() const;
private:
    AILayerHandle layerHandle;
};

#endif /* BtLayer_hpp */

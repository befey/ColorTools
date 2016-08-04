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
    BtLayer& Title(string title);
    
    bool Visible() const;
    BtLayer& Visible(bool visible);
    
    bool Editable() const;
    BtLayer& Editable(bool editable);
    
    bool Printed() const;
    BtLayer& Printed(bool printed);
    
    void DeselectArt() const;
    void SelectArt() const;
    
    long GetArtSet(AIArtSet const targetSet) const;
    void ConvertTextToPaths() const;
    
    void PutArtAtTopOfLayer(AIArtHandle art);
    AIArtHandle GetLayerGroupArt() const;
private:
    AILayerHandle layerHandle;
    
    bool editableWasFalse = false;
    bool visibleWasFalse = false;
    
    void MakeLayerEditableAndStorePreviousState();
    void ApplyStoredAttributes();
};

#endif /* BtLayer_hpp */

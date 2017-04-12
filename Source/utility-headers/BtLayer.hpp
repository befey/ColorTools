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
    BtLayer(){};
    BtLayer(AILayerHandle layer);
    BtLayer(string layerName);
    BtLayer(int layerIndex);
    
    bool Null() const { return !layerHandle; };
    
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
    
    BtLayer& MoveToTop();
    
    BtLayer& MakeEditable();
    BtLayer& ResetEditable();
    
    BtLayer& MakeCurrent();
    
    inline operator const AILayerHandle(void) const { return layerHandle; }
    inline operator AILayerHandle*(void) { return &layerHandle; }
    inline operator bool(void) const { return Null(); }
private:
    AILayerHandle layerHandle = nullptr;
    
    bool storedEditable = true;
    bool storedVisible = true;
};

#endif /* BtLayer_hpp */

//
//  LayerVisibility.hpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#ifndef LayerVisibility_hpp
#define LayerVisibility_hpp

#include "AILayer.h"
#include <map>
#include "BtLayer.hpp"

extern AILayerSuite* sAILayer;

namespace PrintToPdf
{
    class LayerVisibility
    {
    public:
        LayerVisibility();
        bool SetLayerVisibility();
        
    protected:
        map<string, BtLayer> layerList; // Layername, BtLayer
        
    private:
        virtual bool CustomLayerVisibility() = 0;
    };
    
    class BStatLayerVisibility : public LayerVisibility
    {
    public:
        bool CustomLayerVisibility() override;
    };
    
    class LaserLayerVisibility : public LayerVisibility
    {
    public:
        bool CustomLayerVisibility() override;
    };
}

#endif /* LayerVisibility_hpp */

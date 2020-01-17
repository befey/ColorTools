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
#include "SafeguardFileConstants.h"
#include "PrintToPdfConstants.h"

extern AILayerSuite* sAILayer;

namespace PrintToPdf
{
    class LayerVisibility
    {
    public:
        static shared_ptr<LayerVisibility> GetLayerVisibility(SafeguardFile::ProductType productType, PdfPreset preset);
        
        bool SetLayerVisibility() { return CustomLayerVisibility(); };
        
    protected:
        map<string, BtLayer> layerList; // Layername, BtLayer
        
        LayerVisibility();

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
    
    class BStatProofLayerVisibility : public LayerVisibility
    {
    public:
        bool CustomLayerVisibility() override;
    };
    
    class NonStandardLayerVisibility : public LayerVisibility
    {
    public:
        bool CustomLayerVisibility() override;
    };
}

#endif /* LayerVisibility_hpp */

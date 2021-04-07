//
//  ArtObjectFilter.hpp
//  SafeguardTools
//
//  Created by T431962 on 2/9/18.
//
//

#ifndef ArtObjectFilter_hpp
#define ArtObjectFilter_hpp

#include "AIArtSet.h"
#include <vector>
#include "BtArtHandle.hpp"

extern AIArtSetSuite* sAIArtSet;

class ArtObjectFilter
{
public:
    virtual bool Execute() const = 0;
    
    std::vector<BtArtHandle> Results() const { return results; };
protected:
    ArtObjectFilter(AIArtSet* aSet) : artSet(aSet) {};
    ~ArtObjectFilter() {};
    //Members available to subclasses
    
    AIArtSet* artSet;
    std::vector<BtArtHandle> results;
};

class ArtObjectColorFilter : public ArtObjectFilter
{
public:
    ArtObjectColorFilter(AIArtSet* aSet, std::vector<Bt::BtColor> colorList) : ArtObjectFilter(aSet), colors(colorList) {};
    
    bool Execute() const override;
    
private:
    std::vector<Bt::BtColor> colors;
};

#endif /* ArtObjectFilter_hpp */

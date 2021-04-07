//
//  TargetPlate.hpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/29/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#ifndef TargetPlate_h
#define TargetPlate_h

#include "PlateNumber.h"
#include <string>
#include <vector>

class TargetPlate
{
public:
    TargetPlate(std::string plateNum) : pn(plateNum) {};
    
    std::string PlateNumber() const { return pn; };
    
    bool operator == (const TargetPlate& t) const
    {
        return std::string(pn) == std::string(t.pn);
    }
    
    bool operator < (const TargetPlate& t) const
    {
        return std::string(pn) < std::string(t.pn);
    }
private:
    SafeguardFile::PlateNumber pn;
    std::vector<std::string> found;
    std::vector<std::string> deleted;
};

#endif /* TargetPlate_h */

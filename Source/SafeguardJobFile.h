//
//  SafeguardJobFile.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__SafeguardJobFile__
#define __SafeguardTools__SafeguardJobFile__

#include <vector>
#include "Plate.h"
#include "TickMarkDrawer.h"

class SafeguardJobFile
{
public:
    SafeguardJobFile();
    void SetTickMarkDrawer(unique_ptr<PlateBleedInfo::TickMarkDrawer> val) { tickMarkDrawer = move(val); }
    
    vector<Plate> plates;
private:
    unique_ptr<PlateBleedInfo::TickMarkDrawer> tickMarkDrawer;

};

#endif /* defined(__SafeguardTools__SafeguardJobFile__) */

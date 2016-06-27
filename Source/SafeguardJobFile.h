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

namespace SafeguardFile
{
    class SafeguardJobFile
    {
    public:
        void Update();
        void RemoveBleedInfo();
        
    private:
        vector<Plate> plates;
        
        void AddBleedInfo();
        bool ShouldDrawBleedInfo();
    };
}
#endif /* defined(__SafeguardTools__SafeguardJobFile__) */

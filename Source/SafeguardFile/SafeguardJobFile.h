//
//  SafeguardJobFile.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__SafeguardJobFile__
#define __SafeguardTools__SafeguardJobFile__

#include <map>
#include "Plate.h"
#include "PlateNumber.h"
#include "ColorList.h"

namespace PlateBleedInfo
{
    class SafeguardJobFileDTO;
}

namespace SafeguardFile
{
    class SafeguardJobFile
    {
    public:
        SafeguardJobFile();
        SafeguardJobFile(const PlateBleedInfo::SafeguardJobFileDTO* dto);
                
        const PlateNumber GetPlateNumber(int plateIndex = 0) const;
        const string GetToken(int plateIndex = 0) const;
        AIRealRect GetBleeds(int plateIndex = 0) const;
        
        int GetNumPlates() const { return int(plates.size()); };
        
        ColorList GetAllColorsOnJob() const;
        
        void UpdateBleedInfo();
        void UpdateBleedInfo(AIArtHandle pluginArt);
        void RemoveBleedInfo();
        
        const map<int, shared_ptr<Plate>>& GetPlates() const { return plates; };
    private:
        map<int, shared_ptr<Plate>> plates;
    };
}
#endif /* defined(__SafeguardTools__SafeguardJobFile__) */

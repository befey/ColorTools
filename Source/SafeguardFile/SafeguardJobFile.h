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
#include "BleedInfo.h"
#include "ColorList.h"
#include "PrintToPdfConstants.h"
#include "AICSXS.h"

extern AICSXSExtensionSuite* sAICSXSExtension;

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
        
        SafeguardFile::ColorList GetAllColorsOnJob() const;
        
        void UpdateBleedInfo(bool skipCheck = false);
        void EditBleedInfo();
        void RemoveBleedInfo();
        
        const map<int, Plate>& GetPlates() const { return plates; };
    private:
        map<int, Plate> plates;        
    };
}
#endif /* defined(__SafeguardTools__SafeguardJobFile__) */

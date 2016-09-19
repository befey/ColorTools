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
#include "PlateNumber.h"
#include "BleedInfo.h"
#include "ColorList.h"
#include "PrintToPdfConstants.h"
#include "AICSXS.h"
#include "PlateBleedInfoDTO.hpp"

extern AICSXSExtensionSuite* sAICSXSExtension;

namespace SafeguardFile
{
    class SafeguardJobFile
    {
    public:
        SafeguardJobFile();
        
        vector<BleedInfo> GetBleedInfo() const;
        
        const PlateNumber GetPlateNumber(int plateIndex = 0) const;
        const string GetToken(int plateIndex = 0) const;
        AIRealRect GetBleeds(int plateIndex = 0) const;
        
        int GetNumPlates() const { return int(plates.size()); };
        
        SafeguardFile::ColorList GetAllColorsOnJob() const;
        
        void AddBleedInfo();
        void UpdateBleedInfo();
        void EditBleedInfo();
        void RemoveBleedInfo();
        
        void PutDataInDTO(SafeguardFile::PlateBleedInfoDTO& dto);
    private:
        vector<Plate> plates;
        
        void LoadDataFromDTO(SafeguardFile::PlateBleedInfoDTO dto);

    };
}
#endif /* defined(__SafeguardTools__SafeguardJobFile__) */

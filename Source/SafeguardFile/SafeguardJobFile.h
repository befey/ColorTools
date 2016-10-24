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
#include "SafeguardJobFileDTO.hpp"

extern AICSXSExtensionSuite* sAICSXSExtension;

namespace SafeguardFile
{
    class SafeguardJobFile
    {
    public:
        SafeguardJobFile();
        ~SafeguardJobFile();
        
        vector<BleedInfo> GetBleedInfo() const;
        
        const PlateNumber GetPlateNumber(int plateIndex = 0) const;
        const string GetToken(int plateIndex = 0) const;
        AIRealRect GetBleeds(int plateIndex = 0) const;
        
        int GetNumPlates() const { return int(plates.size()); };
        
        SafeguardFile::ColorList GetAllColorsOnJob() const;
        
        void UpdateBleedInfo();
        void EditBleedInfo();
        void RemoveBleedInfo();
        
        void PutDataInDTO(PlateBleedInfoDTO::SafeguardJobFileDTO& dto, bool fullColorName = false);
        void LoadDataFromDTO(PlateBleedInfoDTO::SafeguardJobFileDTO dto);
    private:
        map<int, Plate> plates;
        
        void CleanupPluginArtHandles(vector<AIArtHandle> pluginArts, vector<pair<int, AIArtHandle>> sortedPluginArts) const;
    };
}
#endif /* defined(__SafeguardTools__SafeguardJobFile__) */

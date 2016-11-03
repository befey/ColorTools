//
//  Plate.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__Plate__
#define __SafeguardTools__Plate__

#include "PlateNumber.h"
#include "BleedInfo.h"
#include "PrintToPdfConstants.h"
#include "ColorList.h"
#include "BleedInfoDrawer.h"
#include "AIFolders.h"

extern AIFoldersSuite* sAIFolders;

namespace PlateBleedInfoDTO
{
    class PlateDTO;
}
    
namespace SafeguardFile
{    
    class Plate
    {
    public:
        Plate(ai::ArtboardID id) : Plate(id, NULL) {};
        Plate(ai::ArtboardID id, AIArtHandle pluginArtHandle);
        
        AIRealRect GetArtboardBounds() const;
        tm GetLastModified() const;
        
        string GetArtboardName(bool& isDefault) const;
        const ai::ArtboardID GetArtboardIndex() const { return bleedInfo.ArtboardIndex(); };
        const PlateNumber GetPlateNumber() const;
        const string GetToken() const;
        AIArtHandle GetBleedInfoPluginArtHandle() const { return bleedInfoPluginArt; };
        
        AIRealRect GetBleeds() const;
        SafeguardFile::ColorList GetColors();
        
        BleedInfo& BleedInfo();
        void FillBleedInfoFromPlateDTO(PlateBleedInfoDTO::PlateDTO* dto);
        
        void DrawBleedInfo();
        void RemoveBleedInfo();
        bool ShouldDrawBleedInfo();
    private:
        class BleedInfo bleedInfo;
        AIArtHandle bleedInfoPluginArt = NULL;
        
        shared_ptr<SafeguardFile::BleedInfoDrawer> bleedInfoDrawer;
        
        string GetBleedInfoAsJson(bool fullColorName = false) const;
        void WriteBleedInfoToPluginArt();
    };
}
#endif /* defined(__SafeguardTools__Plate__) */

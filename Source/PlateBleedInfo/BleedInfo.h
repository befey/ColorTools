//
//  BleedInfo.h
//  SafeguardTools
//
//  Created by T431962 on 6/7/16.
//
//

#ifndef __SafeguardTools__BleedInfo__
#define __SafeguardTools__BleedInfo__

#include "AIUser.h"
#include "ColorList.h"
#include "PlateNumber.h"
#include "TickMarkSettings.hpp"
#include <ctime>
#include "AIDocument.h"
#include "AIArtboard.h"

extern AIDocumentSuite* sAIDocument;
extern AIArtboardSuite* sAIArtboard;

namespace PlateBleedInfo
{
    class SafeguardJobFileDTO;
    class PlateDTO;
    
    constexpr auto PLATE_BLEEDINFO =            "__plate_bleedinfo__";
    constexpr auto SG_BLEEDINFO_ARTHANDLES =    "__sg_bleedinfo_arthandles__";
    
    class BleedInfo
    {
    public:
        BleedInfo(ai::ArtboardID artboardIndex) : BleedInfo(artboardIndex, NULL) {};
        BleedInfo(ai::ArtboardID artboardIndex, const PlateBleedInfo::PlateDTO* dto);
        
        ai::ArtboardID ArtboardIndex() const { return artboardIndex; };
        AIRealRect ArtboardBounds() const;
        AIArtHandle BleedInfoPluginArtHandle() const { return bleedInfoPluginArt; };
        BleedInfo& BleedInfoPluginArtHandle(AIArtHandle newVal) { bleedInfoPluginArt = newVal; return *this; };
        tm LastModified() const;
        
        const SafeguardFile::TickMarkSettings TickMarkSettings() const { return tmSettings; };
        BleedInfo& TickMarkStyle(SafeguardFile::TickMarkStyle newVal) { tmSettings.TickMarkStyle(newVal); return *this; };
        
        string Token() const;
        BleedInfo& Token(string newVal);
        string ArtboardName(bool& isDefault) const;
        string ArtboardName() const;
        BleedInfo& ArtboardName(string newVal);
        
        bool ShouldDrawBleedInfo() const { return shouldDrawBleedInfo; };
        BleedInfo& ShouldDrawBleedInfo(bool newVal) { shouldDrawBleedInfo = newVal; return *this; };
        bool ShouldAddCmykBlocks() const { return shouldAddCMYKBlocks; };
        BleedInfo& ShouldAddCmykBlocks(bool newVal) { shouldAddCMYKBlocks = newVal; return *this; };
        const SafeguardFile::PlateNumber PlateNumber() const { return plateNumber; };
        SafeguardFile::ColorList& ColorList() { return colorList; };
        SafeguardFile::ColorList ConstColorList() const { return colorList; };
    
        AIRealRect Bleeds() const;
        
        void StoreInPluginArt() const;
        void ReadFromPluginArt();
        
        void Draw();
        void Remove();
    private:
        bool shouldDrawBleedInfo = true;
        bool shouldAddCMYKBlocks = false;
        
        AIArtHandle bleedInfoPluginArt = NULL;
        
        ai::ArtboardID artboardIndex;
        SafeguardFile::ColorList colorList;
        SafeguardFile::PlateNumber plateNumber;
        SafeguardFile::TickMarkSettings tmSettings;
        
        void SetPlateNumber();
        void SetPlateNumber(string pn);
        
        void FillBleedInfoFromPlateDTO(const PlateBleedInfo::PlateDTO* dto, bool changeArtboardName);
    };
}

#endif /* defined(__SafeguardTools__BleedInfo__) */

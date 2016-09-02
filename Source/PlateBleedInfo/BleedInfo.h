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

namespace SafeguardFile
{
    class BleedInfo
    {
    public:
        BleedInfo() {};
        BleedInfo(ai::ArtboardID artboardIndex);
        
        ai::ArtboardID ArtboardIndex() const { return artboardIndex; };
        AIRealRect ArtboardBounds() const;
        tm LastModified() const;
        
        const TickMarkSettings TickMarkSettings() const { return tmSettings; };
        BleedInfo& TickMarkStyle(TickMarkStyle newVal) { tmSettings.TickMarkStyle(newVal); return *this;};
        
        string Token() const;
        BleedInfo& Token(string newVal);
        string ArtboardName(bool& isDefault) const;
        BleedInfo& ArtboardName(string newVal);
        
        bool AddCmykBlocks() const { return shouldAddCMYKBlocks; };
        BleedInfo& AddCmykBlocks(bool newVal) { shouldAddCMYKBlocks = newVal; return *this;};
        
        bool ShouldDrawBleedInfo() const { return shouldDrawBleedInfo; };
        BleedInfo& ShouldDrawBleedInfo(bool newVal) { shouldDrawBleedInfo = newVal; return *this;};
        
        const SafeguardFile::PlateNumber PlateNumber() const { return plateNumber; };
        const SafeguardFile::ColorList ColorList() const { return colorList; };
        bool SetColorMethod(string colorName, SafeguardFile::InkMethod method);
    
        AIRealRect Bleeds() const;
        
    private:
        bool shouldDrawBleedInfo = true;
        ai::ArtboardID artboardIndex;
        SafeguardFile::ColorList colorList;
        SafeguardFile::PlateNumber plateNumber;
        bool shouldAddCMYKBlocks = true;
        SafeguardFile::TickMarkSettings tmSettings;
        
        void AddColorsOfArtToColorList(AIArtHandle art);
        void FillColorList();
        
        void SetPlateNumber();
        void SetPlateNumber(string pn);
    };
}

#endif /* defined(__SafeguardTools__BleedInfo__) */
//
//  Plate.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__Plate__
#define __SafeguardTools__Plate__

#include "AIPathStyle.h"
#include "AIFont.h"
#include "AIArtboard.h"
#include "AIDocument.h"
#include "PlateNumber.h"
#include "BleedInfo.h"
#include "PrintToPdfConstants.h"
#include "ColorList.h"
#include "BleedInfoDrawer.h"
#include <vector>

extern AIPathStyleSuite* sAIPathStyle;
extern AIFontSuite* sAIFont;
extern AIArtboardSuite* sAIArtboard;
extern AIDocumentSuite* sAIDocument;

namespace SafeguardFile
{
    constexpr auto KEYLINE_LAYER =              "KEY LINE";
    constexpr auto REG_LAYER =                  "Registration Guide for Proof";
    constexpr auto SLUG_LAYER =                 "SLUG";
    constexpr auto FOREGROUND_LAYER =           "Foreground";
    constexpr auto GUIDE_LAYER =                "Guides";
    constexpr auto BACKGROUND_LAYER =           "Background";
    
    static constexpr auto NO_TOKEN_DESIG =      "F";
    
    class Plate
    {
    public:
        Plate(ai::ArtboardID id);
        //Plate(ai::ArtboardID id, string pn);
        
        AIRealRect GetArtboardBounds() const;
        AIUserDateTime GetLastModified() const;
        
        string GetArtboardName(AIBoolean& isDefault) const;
        const ai::ArtboardID GetArtboardIndex() const { return bleedInfo.artboardIndex; };
        const PlateNumber GetPlateNumber() const;
        const string GetToken() const;
        void SetPlateNumber();
        
        AIRealRect GetBleeds(PrintToPdf::PdfPreset preset) const;
        
        void AddBleedInfo();
        void RemoveBleedInfo();
    private:
        BleedInfo bleedInfo;
        shared_ptr<SafeguardFile::BleedInfoDrawer> bleedInfoDrawer;
        
        void SetPlateNumber(string pn);
        string CreateToken() const;
        
        void AddColorsOfArtToColorList(AIArtHandle art);
        void FillColorList();
    };
}
#endif /* defined(__SafeguardTools__Plate__) */

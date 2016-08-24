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
#include <ctime>

extern AIPathStyleSuite* sAIPathStyle;
extern AIFontSuite* sAIFont;
extern AIArtboardSuite* sAIArtboard;
extern AIDocumentSuite* sAIDocument;

namespace SafeguardFile
{    
    class Plate
    {
    public:
        Plate(ai::ArtboardID id);
        
        AIRealRect GetArtboardBounds() const;
        tm GetLastModified() const;
        
        string GetArtboardName(bool& isDefault) const;
        const ai::ArtboardID GetArtboardIndex() const { return bleedInfo.ArtboardIndex(); };
        const PlateNumber GetPlateNumber() const;
        const string GetToken() const;
        
        AIRealRect GetBleeds() const;
        
        BleedInfo BleedInfo() const;
        Plate& BleedInfo(class BleedInfo newValue) { bleedInfo = newValue; };
        
        void DrawBleedInfo();
        void RemoveBleedInfo();
    private:
        class BleedInfo bleedInfo;
        shared_ptr<SafeguardFile::BleedInfoDrawer> bleedInfoDrawer;
    };
}
#endif /* defined(__SafeguardTools__Plate__) */

//
//  PlateNumber.h
//  SafeguardTools
//
//  Created by T431962 on 4/7/16.
//
//

#ifndef __SafeguardTools__PlateNumber__
#define __SafeguardTools__PlateNumber__

/*
 
 enum PlantCodes {
 B,
 C,
 E,
 F,
 H,
 K,
 L,
 M,
 S,
 T,
 V,
 W,
 X,
 Y
 };
 
 */

#include <string>
#include "SafeguardFileConstants.h"

#include "AIArt.h"
#include "AIArtSet.h"
#include "AIPath.h"
#include "AIArtboard.h"
#include "AIRealMath.h"

extern AIArtSuite* sAIArt;
extern AIArtSetSuite* sAIArtSet;
extern AIPathSuite* sAIPath;
extern AIArtboardSuite* sAIArtboard;
extern AIRealMathSuite* sAIRealMath;

namespace SafeguardFile
{
    class PlateNumber
    {
    public:
        PlateNumber(){};
        PlateNumber(std::string);
        
        inline bool IsValid() const {return isValidPlateNumber;};
        
        inline std::string GetPlantIndicator() const {return plantIndicator;};
        inline std::string GetProductIndicator() const {return productIndicator;};
        ProductType GetProductType() const;
        inline std::string GetSuffix() const { return suffix; };
        
        inline operator std::string(void) const { return plateNumber; }
        
    private:
        std::string plateNumber;
        std::string plantIndicator;
        std::string year;
        std::string productIndicator;
        std::string number;
        std::string suffix;
        bool isValidPlateNumber = false;
        
        bool TokenizePlateNumber();
        
        bool HasInnerTicks() const;
    };
    
}

#endif /* defined(__SafeguardTools__PlateNumber__) */

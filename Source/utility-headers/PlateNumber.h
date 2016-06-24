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
#include "AIArtboard.h"
extern AIArtSuite* sAIArt;
extern AIPathSuite* sAIPath;
extern AIArtSetSuite* sAIArtSet;
extern AIArtboardSuite* sAIArtboard;
extern AIRealMathSuite* sAIRealMath;

class PlateNumber
{
public:
    enum ProductType {
        INVAL,
        CutSheet,
        Continuous,
        Snapset,
        BusinessStat
    };
    
    static constexpr auto LENGTH_OF_INNER_TICK_PATH = 18;
    
    PlateNumber(){};
    PlateNumber(string);
    
    inline Boolean IsValid() const {return isValidPlateNumber;};
    string GetPlateNumber() const;
    inline string GetPlantIndicator() const {return plantIndicator;};
    inline string GetProductIndicator() const {return productIndicator;};
    ProductType GetProductType() const;
    
    void GetAsTextRange(ATE::ITextRange& targetRange) const;
    
private:
    string plateNumber;
    string plantIndicator;
    string year;
    string productIndicator;
    string number;
    Boolean isValidPlateNumber = false;
    
    Boolean TokenizePlateNumber();
    Boolean HasInnerTicks() const;
};

#endif /* defined(__SafeguardTools__PlateNumber__) */

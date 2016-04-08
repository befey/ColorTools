//
//  PlateNumber.h
//  SafeguardTools
//
//  Created by T431962 on 4/7/16.
//
//

#ifndef __SafeguardTools__PlateNumber__
#define __SafeguardTools__PlateNumber__

/*#include <unordered_map>

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

enum ProductTypes {
    
};
*/
class PlateNumber
{
public:
    PlateNumber(string);
    PlateNumber(ai::UnicodeString);
    inline Boolean IsValid() {return isValidPlateNumber;};
    inline string GetPlateNumber() {return plateNumber;};
    inline string GetPlantIndicator() {return plantIndicator;};
    inline string GetProductIndicator() {return productIndicator;};
    
private:
    string plateNumber;
    string plantIndicator;
    string year;
    string productIndicator;
    string number;
    Boolean isValidPlateNumber = false;
    
    Boolean TokenizePlateNumber();
};

#endif /* defined(__SafeguardTools__PlateNumber__) */

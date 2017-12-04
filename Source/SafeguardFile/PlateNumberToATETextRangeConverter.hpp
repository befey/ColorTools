//
//  PlateNumberToATETextRangeConverter.hpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/28/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#ifndef PlateNumberToATETextRangeConverter_hpp
#define PlateNumberToATETextRangeConverter_hpp

#include "AIPath.h"
#include "AIArtboard.h"

extern AIPathSuite* sAIPath;
extern AIArtboardSuite* sAIArtboard;

#include "PlateNumber.h"

namespace SafeguardFile
{
    class PlateNumberToATETextRangeConverter
    {
    public:
        PlateNumberToATETextRangeConverter(){};
        
    private:
        PlateNumber pn;
        
        void GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const override;
    };
    
#endif /* PlateNumberToATETextRangeConverter_hpp */

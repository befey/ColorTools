//
//  BtArtboardRange.h
//  SafeguardTools
//
//  Created by T431962 on 4/12/16.
//
//

#ifndef __SafeguardTools__BtArtboardRange__
#define __SafeguardTools__BtArtboardRange__

#include "AIArtboardRange.h"
#include "AIArtboard.h"

extern "C" AIArtboardRangeSuite* sAIArtboardRange;
extern "C" AIArtboardSuite* sAIArtboard;


class BtArtboardRange
{
public:
    BtArtboardRange(string);
    ~BtArtboardRange();
    
    inline operator string(void) const {return rangeS;}
    inline operator AIArtboardRangeHandle(void) const {return range;}
    
private:
    string rangeS;
    AIArtboardRangeHandle range;
    bool isValid = false;
};

#endif /* defined(__SafeguardTools__BtArtboardRange__) */

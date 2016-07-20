//
//  BtArtboardRange.h
//  SafeguardTools
//
//  Created by T431962 on 4/12/16.
//
//

#ifndef __SafeguardTools__BtArtboardRange__
#define __SafeguardTools__BtArtboardRange__

//=================================
// included dependencies
#include "AIArtboardRange.h"
#include "AIArtboard.h"

//=================================
// forward declared dependencies
extern AIArtboardRangeSuite* sAIArtboardRange;
extern AIArtboardSuite* sAIArtboard;

//=================================
// BtAiMenuItemHandles - manages a standard range notation ie. 1-2,4
class BtArtboardRange
{
public:
    BtArtboardRange(string);
    ~BtArtboardRange();
    BtArtboardRange(const BtArtboardRange& src);
    BtArtboardRange& operator=(const BtArtboardRange& rhs);
    
    inline operator string(void) const {return rangeS;}
    inline operator AIArtboardRangeHandle(void) const {return range;}
    
private:
    string rangeS;
    AIArtboardRangeHandle range;
    bool isValid = false;
};

#endif /* defined(__SafeguardTools__BtArtboardRange__) */

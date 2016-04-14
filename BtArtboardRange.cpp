//
//  BtArtboardRange.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/12/16.
//
//

#include "BtArtboardRange.h"

BtArtboardRange::BtArtboardRange(string r) : rangeS(r)
{
    ai::UnicodeString rUS = ai::UnicodeString(rangeS);
    if (kBadParameterErr == sAIArtboardRange->ValidateString(&rUS)) {
        return;
    }
    
    isValid = true;
    
    if (rangeS.length() == 0) {
        ai::int32 count;
        ai::ArtboardList abList;
        sAIArtboard->GetArtboardList(abList);
        sAIArtboard->GetCount(abList, count);
        
        rangeS = "1-" + to_string(count);
        rUS = ai::UnicodeString(rangeS);
        
        sAIArtboardRange->Create(TRUE, &rUS, &range); //ALL ARTBOARDS
    }
    else
    {
        sAIArtboardRange->Create(FALSE, &rUS, &range); //RANGE
    }
}

BtArtboardRange::~BtArtboardRange()
{
    sAIArtboardRange->Dispose(range);
}
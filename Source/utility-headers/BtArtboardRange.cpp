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
    AIErr err;
    ai::UnicodeString rUS = ai::UnicodeString(rangeS);
    if (kBadParameterErr == sAIArtboardRange->ValidateString(&rUS)) {
        rangeS = ""; //If a bad string is input, default to all pages
    }
    
    isValid = true;
    
    if (rangeS.length() == 0) {
        ai::int32 count;
        ai::ArtboardList abList;
        sAIArtboard->GetArtboardList(abList);
        sAIArtboard->GetCount(abList, count);
        
        rangeS = "1-" + to_string(count);
        rUS = ai::UnicodeString(rangeS);
        
        err = sAIArtboardRange->Create(TRUE, &rUS, &range); //ALL ARTBOARDS
    }
    else
    {
        err = sAIArtboardRange->Create(FALSE, &rUS, &range); //RANGE
    }
}

BtArtboardRange::~BtArtboardRange()
{
    sAIArtboardRange->Dispose(range);
}

BtArtboardRange::BtArtboardRange(const BtArtboardRange& src) : rangeS(src.rangeS)
{
    AIErr err;
    ai::UnicodeString rUS = ai::UnicodeString(rangeS);
    if (kBadParameterErr == sAIArtboardRange->ValidateString(&rUS)) {
        rangeS = ""; //If a bad string is input, default to all pages
    }
    
    isValid = true;
    
    if (rangeS.length() == 0) {
        ai::int32 count;
        ai::ArtboardList abList;
        sAIArtboard->GetArtboardList(abList);
        sAIArtboard->GetCount(abList, count);
        
        rangeS = "1-" + to_string(count);
        rUS = ai::UnicodeString(rangeS);
        
        err = sAIArtboardRange->Create(TRUE, &rUS, &range); //ALL ARTBOARDS
    }
    else
    {
        err = sAIArtboardRange->Create(FALSE, &rUS, &range); //RANGE
    }
}

BtArtboardRange& BtArtboardRange::operator=(const BtArtboardRange& rhs)
{
    if (&rhs != this)
    {
        rangeS = rhs.rangeS;
        
        AIErr err;
        ai::UnicodeString rUS = ai::UnicodeString(rangeS);
        if (kBadParameterErr == sAIArtboardRange->ValidateString(&rUS)) {
            rangeS = ""; //If a bad string is input, default to all pages
        }
        
        isValid = true;
        
        if (rangeS.length() == 0) {
            ai::int32 count;
            ai::ArtboardList abList;
            sAIArtboard->GetArtboardList(abList);
            sAIArtboard->GetCount(abList, count);
            
            rangeS = "1-" + to_string(count);
            rUS = ai::UnicodeString(rangeS);
            
            err = sAIArtboardRange->Create(TRUE, &rUS, &range); //ALL ARTBOARDS
        }
        else
        {
            err = sAIArtboardRange->Create(FALSE, &rUS, &range); //RANGE
        }
    }
    return *this;
}

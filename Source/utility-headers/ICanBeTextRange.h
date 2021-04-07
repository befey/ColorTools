//
//  ICanBeTextRange.h
//  SafeguardTools
//
//  Created by T431962 on 11/15/16.
//
//

#ifndef ICanBeTextRange_h
#define ICanBeTextRange_h

#include "AIRealMath.h"

class ICanBeTextRange
{
public:
    void AsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth = kAIRealMax) const { GetAsTextRange(targetRange, maxWidth); };
private:
    virtual void GetAsTextRange(ATE::ITextRange& targetRange, AIReal maxWidth) const = 0;
};

#endif /* ICanBeTextRange_h */

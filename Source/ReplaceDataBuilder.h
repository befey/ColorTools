//
//  ReplaceDataBuilder.h
//  SafeguardTools
//
//  Created by T431962 on 4/21/16.
//
//

#ifndef __SafeguardTools__ReplaceDataBuilder__
#define __SafeguardTools__ReplaceDataBuilder__

#include "ReplaceData.h"


class ReplaceDataBuilder
{
public:
    static ReplaceDataBuilder* ReplaceDataObj() { return new ReplaceDataBuilder(); }
    ReplaceDataBuilder* Action(ReplaceData::AddRemove action) { replaceData->SetAddRemove(action); return this; }
    ReplaceDataBuilder* Attribute(ReplaceData::Attribute attr) { replaceData->SetAttribute(attr); return this; }
    ReplaceDataBuilder* In(ReplaceData::ApplyTo sf) { replaceData->SetApplyTo(sf); return this; }
    ReplaceDataBuilder* On(ReplaceData::ChangeIn ds) { replaceData->SetChangeIn(ds); return this; }
    ReplaceDataBuilder* From(AIColor from) { replaceData->SetFromColor(from); return this; }
    ReplaceDataBuilder* To(AIColor to) { replaceData->SetToColor(to); return this; }
    ReplaceDataBuilder* WithTints(bool t) {replaceData->SetChangeTints(t); return this; };
    
    ReplaceData* Build() { return replaceData; }
private:
    ReplaceDataBuilder() : replaceData(new ReplaceData()) {}
    ReplaceData* const replaceData;
    
};


#endif /* defined(__SafeguardTools__ReplaceDataBuilder__) */

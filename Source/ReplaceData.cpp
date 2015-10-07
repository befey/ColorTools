//
//  ReplaceData.cpp
//  ColorTools
//
//  Created by T431962 on 10/7/15.
//
//

#include "ReplaceData.h"
#include "rapidjson/document.h"


ReplaceData::ReplaceData(const char* eventData)
{
    using namespace rapidjson;
    
    Document d;
    d.Parse(eventData);
    
    Value& v = d[ATTRIBUTE_SELECT];
    attributeSelect = v.GetInt();
    
    v = d[CHANGEIN_SELECT];
    changeinSelect = v.GetInt();
    
    v = d[ADDREMOVE_SELECT];
    addremoveSelect = v.GetInt();
    
    v = d[APPLYTO_SELECT];
    applytoSelect = v.GetInt();

    v = d[FROM_SELECT];
    fromSelect = v.GetString();
    
    v = d[TO_SELECT];
    toSelect = v.GetString();
    
    v = d[TINTS_CHECKBOX];
    tintsCheckbox = v.GetBool();
}
//
//  ReplaceData.cpp
//  ColorTools
//
//  Created by T431962 on 10/7/15.
//
//

#include "ReplaceData.h"
#include "document.h"

#include "ColorFuncs.h"


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
    
    v = d[TINTS_CHECKBOX];
    tintsCheckbox = v.GetBool();
    
    
    v = d[FROM_SELECT];
    fromSelect = v.GetString();
    
    v = d[TO_SELECT];
    toSelect = v.GetString();
    
    
    //Set controlFlags based on Strokes and Fills
    controlFlags = kVisitColorsUniversally |  kVisitGlobalObjectsOnceOnly; //APPLYTO_FILLSANDSTROKES
    //Set the VisitFlags based on the apply to and whatnot
    if ( applytoSelect == APPLYTO_STROKES )
    {
        controlFlags = controlFlags | kVisitColorsStrokesOnly;
    }
    if ( applytoSelect == APPLYTO_FILLS )
    {
        controlFlags = controlFlags | kVisitColorsFillsOnly;
    }
    
    //CREATE THE HANDLES and color specs FOR THE TO AND FROM COLORS
    SetColorByName( fromSelect , *fromColor );
    SetColorByName( toSelect , *toColor );
}
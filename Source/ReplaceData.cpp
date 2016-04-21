//
//  ReplaceData.cpp
//  SafeguardTools
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
    attribute = static_cast<Attribute>(v.GetInt());
    
    v = d[CHANGEIN_SELECT];
    changein = static_cast<ChangeIn>(v.GetInt());
    
    v = d[ADDREMOVE_SELECT];
    addremove = static_cast<AddRemove>(v.GetInt());
    
    v = d[APPLYTO_SELECT];
    applyto = static_cast<ApplyTo>(v.GetInt());
    
    v = d[TINTS_CHECKBOX];
    changeTints = v.GetBool();
    
    
    v = d[FROM_SELECT];
    fromString = v.GetString();
    
    v = d[TO_SELECT];
    toString = v.GetString();
    
    
    //Set controlFlags based on Strokes and Fills
    controlFlags = kVisitColorsUniversally |  kVisitGlobalObjectsOnceOnly; //APPLYTO_FILLSANDSTROKES
    //Set the VisitFlags based on the apply to and whatnot
    if ( applyto == ApplyTo::Strokes )
    {
        controlFlags = controlFlags | kVisitColorsStrokesOnly;
    }
    if ( applyto == ApplyTo::Fills )
    {
        controlFlags = controlFlags | kVisitColorsFillsOnly;
    }
    
    //CREATE THE HANDLES and color specs FOR THE TO AND FROM COLORS
    SetColorByName( fromString , fromColor );
    SetColorByName( toString , toColor );
}
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
    SetAttribute(static_cast<Attribute>(v.GetInt()));
    
    v = d[CHANGEIN_SELECT];
    SetChangeIn(static_cast<ChangeIn>(v.GetInt()));
    
    v = d[ADDREMOVE_SELECT];
    SetAddRemove(static_cast<AddRemove>(v.GetInt()));
    
    v = d[APPLYTO_SELECT];
    SetApplyTo(static_cast<ApplyTo>(v.GetInt()));
    
    v = d[TINTS_CHECKBOX];
    SetChangeTints(v.GetBool());
    
    
    v = d[FROM_SELECT];
    SetFromColor(v.GetString());
    
    v = d[TO_SELECT];
    SetToColor(v.GetString());
}


void ReplaceData::SetAddRemove(AddRemove ar)
{
    addremove = ar;
}

void ReplaceData::SetAttribute(Attribute attr)
{
    attribute = attr;
}

void ReplaceData::SetApplyTo(ApplyTo at)
{
    applyto = at;
    
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
}

void ReplaceData::SetChangeIn(ChangeIn ci)
{
    changein = ci;
}

void ReplaceData::SetChangeTints(bool ct)
{
    changeTints = ct;
}

void ReplaceData::SetFromColor(string s)
{
    AIColor c;
    SetColorByName( s, c );
    SetFromColor(c);
}

void ReplaceData::SetFromColor(AIColor fc)
{
    fromColor = fc;
}

void ReplaceData::SetToColor(string s)
{
    AIColor c;
    SetColorByName( s, c );
    SetToColor(c);
}

void ReplaceData::SetToColor(AIColor tc)
{
    toColor = tc;
}
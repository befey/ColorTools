//
//  ReplaceData.cpp
//  SafeguardTools
//
//  Created by T431962 on 10/7/15.
//
//

#include "ReplaceData.h"
#include "rapidjson/document.h"

#include "ColorFuncs.h"


ReplaceData::ReplaceData(const char* eventData)
{
    using namespace rapidjson;
    
    Document d;
    d.Parse(eventData);
   
    Value& v = d[ColorToolsUIController::ATTRIBUTE_SELECT];
    SetAttribute(static_cast<ColorToolsUIController::Attribute>(v.GetInt()));
    
    v = d[ColorToolsUIController::CHANGEIN_SELECT];
    SetChangeIn(static_cast<ColorToolsUIController::ChangeIn>(v.GetInt()));
    
    v = d[ColorToolsUIController::ADDREMOVE_SELECT];
    SetAddRemove(static_cast<ColorToolsUIController::AddRemove>(v.GetInt()));
    
    v = d[ColorToolsUIController::APPLYTO_SELECT];
    SetApplyTo(static_cast<ColorToolsUIController::ApplyTo>(v.GetInt()));
    
    v = d[ColorToolsUIController::TINTS_CHECKBOX];
    SetChangeTints(v.GetBool());
    
    
    v = d[ColorToolsUIController::FROM_SELECT];
    SetFromColor(v.GetString());
    
    v = d[ColorToolsUIController::TO_SELECT];
    SetToColor(v.GetString());
}


void ReplaceData::SetAddRemove(ColorToolsUIController::AddRemove ar)
{
    addremove = ar;
}

void ReplaceData::SetAttribute(ColorToolsUIController::Attribute attr)
{
    attribute = attr;
}

void ReplaceData::SetApplyTo(ColorToolsUIController::ApplyTo at)
{
    applyto = at;
    
    //Set controlFlags based on Strokes and Fills
    controlFlags = kVisitColorsUniversally |  kVisitGlobalObjectsOnceOnly | kVisitColorsIncludeRegistration; //APPLYTO_FILLSANDSTROKES
    //Set the VisitFlags based on the apply to and whatnot
    if ( applyto == ColorToolsUIController::ApplyTo::Strokes )
    {
        controlFlags = controlFlags | kVisitColorsStrokesOnly;
    }
    if ( applyto == ColorToolsUIController::ApplyTo::Fills )
    {
        controlFlags = controlFlags | kVisitColorsFillsOnly;
    }
}

void ReplaceData::SetChangeIn(ColorToolsUIController::ChangeIn ci)
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
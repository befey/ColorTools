//
//  ReplaceData.h
//  SafeguardTools
//
//  Created by T431962 on 10/7/15.
//
//

#ifndef __SafeguardTools__ReplaceData__
#define __SafeguardTools__ReplaceData__

#include "ColorToolsUIController.h"

class ReplaceDataBuilder;

//=================================
// ReplaceData - contains params required for a Find/Replace operation
class ReplaceData
{
public:
    ReplaceData(const char* eventData);
    
    void SetAddRemove(ColorToolsUIController::AddRemove);
    void SetAttribute(ColorToolsUIController::Attribute);
    void SetApplyTo(ColorToolsUIController::ApplyTo);
    void SetChangeIn(ColorToolsUIController::ChangeIn);
    void SetChangeTints(bool);
    void SetFromColor(string);
    void SetFromColor(AIColor);
    void SetToColor(string);
    void SetToColor(AIColor);
    
    friend class ReplaceDataBuilder;
    //TODO: rework these functions so they don't need private access
    friend int FindAndReplace(ReplaceData* data);
    friend void adjustColor(AIColor *color, void* userData, AIErr *result, AIBoolean *altered);
private:
    ReplaceData();
    
    ColorToolsUIController::Attribute attribute;
    ColorToolsUIController::ChangeIn changein;
    string fromString;
    string toString;
    ColorToolsUIController::AddRemove addremove;
    ColorToolsUIController::ApplyTo applyto;
    bool changeTints;
    
    VisitAIColorFlags controlFlags = kVisitColorsNullFlags;
    AIColor fromColor;
    AIColor toColor;
};


#endif /* defined(__SafeguardTools__ReplaceData__) */

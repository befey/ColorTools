//
//  ReplaceData.h
//  SafeguardTools
//
//  Created by T431962 on 10/7/15.
//
//

#ifndef __SafeguardTools__ReplaceData__
#define __SafeguardTools__ReplaceData__


//=================================
// ReplaceData - contains the params received from the Find and Replace extension
class ReplaceData
{
public:
    ReplaceData(const char* eventData);
    
    //=================================
    // Constant definitions
    static constexpr auto ATTRIBUTE_SELECT =    "attribute-select";
    enum class Attribute
    {
        Color =                                 0,
        Overprint =                             1
    };
    
    static constexpr auto CHANGEIN_SELECT =     "changein-select";
    enum class ChangeIn
    {
        Selection =                             0,
        Document =                              1
    };
    
    static constexpr auto ADDREMOVE_SELECT =    "addremove-select";
    enum class AddRemove
    {
        Add =                                   0,
        Remove =                                1
    };
    
    static constexpr auto APPLYTO_SELECT =      "applyto-select";
    enum class ApplyTo
    {
        FillsAndStrokes =                       0,
        Fills =                                 1,
        Strokes =                               2
    };
        
    static constexpr auto FROM_SELECT =         "from-select";
    static constexpr auto TO_SELECT =           "to-select";
    static constexpr auto TINTS_CHECKBOX =      "tints-checkbox";

    Attribute attribute;
    ChangeIn changein;
    string fromString;
    string toString;
    AddRemove addremove;
    ApplyTo applyto;
    bool changeTints;
    
    VisitAIColorFlags controlFlags = kVisitColorsNullFlags;
    AIColor fromColor;
    AIColor toColor;
};


#endif /* defined(__SafeguardTools__ReplaceData__) */

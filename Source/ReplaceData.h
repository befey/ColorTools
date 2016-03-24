//
//  ReplaceData.h
//  SafeguardTools
//
//  Created by T431962 on 10/7/15.
//
//

#ifndef __SafeguardTools__ReplaceData__
#define __SafeguardTools__ReplaceData__

#define ATTRIBUTE_SELECT                     "attribute-select"
#define ATTRIBUTE_COLOR                     0
#define ATTRIBUTE_OVERPRINT                 1

#define CHANGEIN_SELECT                     "changein-select"
#define CHANGEIN_SELECTION                  0
#define CHANGEIN_DOCUMENT                   1

#define ADDREMOVE_SELECT                    "addremove-select"
#define ADDREMOVE_ADD                       0
#define ADDREMOVE_REMOVE                    1

#define APPLYTO_SELECT                      "applyto-select"
#define APPLYTO_FILLSANDSTROKES             0
#define APPLYTO_FILLS                       1
#define APPLYTO_STROKES                     2

#define FROM_SELECT                         "from-select"
#define TO_SELECT                           "to-select"
#define TINTS_CHECKBOX                      "tints-checkbox"


class ReplaceData
{
public:
    ReplaceData(const char* eventData);
    
    int attributeSelect;
    int changeinSelect;
    string fromSelect;
    string toSelect;
    int addremoveSelect;
    int applytoSelect;
    bool tintsCheckbox;
    
    VisitAIColorFlags controlFlags = kVisitColorsNullFlags;
    AIColor fromColor;
    AIColor toColor;
};


#endif /* defined(__SafeguardTools__ReplaceData__) */

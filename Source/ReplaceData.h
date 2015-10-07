//
//  ReplaceData.h
//  ColorTools
//
//  Created by T431962 on 10/7/15.
//
//

#ifndef __ColorTools__ReplaceData__
#define __ColorTools__ReplaceData__


#define ATTRIBUTE_COLOR                     0
#define ATTRIBUTE_OVERPRINT                 1
#define CHANGEIN_SELECTION                  0
#define CHANGEIN_DOCUMENT                   1
#define ADDREMOVE_ADD                       0
#define ADDREMOVE_REMOVE                    1
#define APPLYTO_FILLSANDSTROKES             0
#define APPLYTO_FILLS                       1
#define APPLYTO_STROKES                     2


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
};


#endif /* defined(__ColorTools__ReplaceData__) */

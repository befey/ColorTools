//
//  ArtboardNameRetriever.hpp
//  SafeguardTools
//
//  Created by T431962 on 12/2/16.
//
//

#ifndef ArtboardNameRetriever_hpp
#define ArtboardNameRetriever_hpp

#include "AIArtboard.h"

extern AIArtboardSuite* sAIArtboard;

class ArtboardNameRetriever
{
public:
    static string GetName(int artboardIndex, bool& isDefault)
    {
        ai::ArtboardList abList;
        sAIArtboard->GetArtboardList(abList);
        ai::ArtboardProperties props;
        sAIArtboard->GetArtboardProperties(abList, artboardIndex, props);
        ai::UnicodeString abName;
        props.GetName(abName);
        string abNameS = abName.getInStdString(kAIPlatformCharacterEncoding);
        
        AIBoolean aiboolIsDefault;
        sAIArtboard->IsDefaultName(props, aiboolIsDefault);
        isDefault = aiboolIsDefault;
        
        return abNameS;
    }
    
    static string GetName(int artboardIndex)
    {
        bool aiboolIsDefault = false;
        return GetName(artboardIndex, aiboolIsDefault);
    }
};


#endif /* ArtboardNameRetriever_hpp */

//
//  PreferenceWriter.hpp
//  SafeguardTools
//
//  Created by T431962 on 2/3/17.
//
//

#ifndef PreferenceWriter_hpp
#define PreferenceWriter_hpp

#include "AIPreference.h"

extern AIPreferenceSuite* sAIPreference;

class PreferenceWriter
{
public:
    PreferenceWriter(string prefix) : prefix(prefix) {};
    
    bool GetFilePathFromIdentifier(string identifier, ai::FilePath& path);
    bool SetFilePathForIdentifier(string identifier, ai::FilePath path);
private:
    string prefix;
};

#endif /* PreferenceRetriever_hpp */

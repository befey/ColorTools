//
//  AiPreferenceWriter.hpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/29/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#ifndef AiPreferenceWriter_h
#define AiPreferenceWriter_h

#include "AIPreference.h"

extern AIPreferenceSuite* sAIPreference;

class AiPreferenceWriter : public PreferenceWriter
{
public:
    AiPreferenceWriter(std::string prefix) : PreferenceWriter(prefix) {};
    
    bool GetFilePathFromIdentifier(string identifier, boost::filesystem::path& path) override
    {
        sAIPreference->GetFilePathSpecificationPreference(prefix.c_str(), identifier.c_str(), path);
        return !path.IsEmpty();
    }
    
    bool SetFilePathForIdentifier(string identifier, boost::filesystem::path path) override
    {
        ASErr err = sAIPreference->PutFilePathSpecificationPreference(prefix.c_str(), identifier.c_str(), path);
        return !err;
    }
};

#endif /* AiPreferenceWriter_h */

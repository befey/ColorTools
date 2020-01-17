//
//  AiPreferenceWriter.hpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/29/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#ifndef AiPreferenceWriter_h
#define AiPreferenceWriter_h

#include "PreferenceWriter.hpp"
#include "AIPreference.h"

extern AIPreferenceSuite* sAIPreference;

class AiPreferenceWriter : public PreferenceWriter
{
public:
    AiPreferenceWriter(std::string prefix) : PreferenceWriter(prefix) {};
    
    bool GetFilePathFromIdentifier(string identifier, boost::filesystem::path& path) override
    {
        ai::FilePath aiFp(ai::UnicodeString(path.string()));
        sAIPreference->GetFilePathSpecificationPreference(prefix.c_str(), identifier.c_str(), aiFp);
        path = aiFp.GetFullPath().as_Platform();
        return !path.empty();
    }
    
    bool SetFilePathForIdentifier(string identifier, boost::filesystem::path path) override
    {
        ai::FilePath aiFp(ai::UnicodeString(path.string()));
        ASErr err = sAIPreference->PutFilePathSpecificationPreference(prefix.c_str(), identifier.c_str(), aiFp);
        return !err;
    }
};

#endif /* AiPreferenceWriter_h */

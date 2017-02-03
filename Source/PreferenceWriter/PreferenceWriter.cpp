//
//  PreferenceWriter.cpp
//  SafeguardTools
//
//  Created by T431962 on 2/3/17.
//
//

#include "PreferenceWriter.hpp"

bool PreferenceWriter::GetFilePathFromIdentifier(string identifier, ai::FilePath& path)
{
    sAIPreference->GetFilePathSpecificationPreference(prefix.c_str(), identifier.c_str(), path);
    return !path.IsEmpty();
}

bool PreferenceWriter::SetFilePathForIdentifier(string identifier, ai::FilePath path)
{
    ASErr err = sAIPreference->PutFilePathSpecificationPreference(prefix.c_str(), identifier.c_str(), path);
    return !err;
}

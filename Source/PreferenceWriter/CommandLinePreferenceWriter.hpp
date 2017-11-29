//
//  CommandLinePreferenceWriter.hpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/29/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#ifndef CommandLinePreferenceWriter_h
#define CommandLinePreferenceWriter_h

#include "PreferenceWriter.hpp"
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

class CommandLinePreferenceWriter : public PreferenceWriter
{
public:
    CommandLinePreferenceWriter(std::string prefix) : PreferenceWriter(prefix) {};
    
    bool GetFilePathFromIdentifier(string identifier, boost::filesystem::path& path) override
    {
        return true;
    }
    
    bool SetFilePathForIdentifier(string identifier, boost::filesystem::path path) override
    {
        return true;
    }
};


#endif /* CommandLinePreferenceWriter_h */

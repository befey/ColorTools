//
//  PreferenceWriter.hpp
//  SafeguardTools
//
//  Created by T431962 on 2/3/17.
//
//

#ifndef PreferenceWriter_hpp
#define PreferenceWriter_hpp

#include <string>
#include <boost/filesystem.hpp>

class PreferenceWriter
{
public:
    virtual bool GetFilePathFromIdentifier(std::string identifier, boost::filesystem::path& path) = 0;
    virtual bool SetFilePathForIdentifier(std::string identifier, boost::filesystem::path path) = 0;
    
protected:
    PreferenceWriter(std::string prefix) : prefix(prefix) {};
    
    std::string prefix;
};



#endif /* PreferenceRetriever_hpp */

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
    virtual bool GetFilePathFromIdentifier(std::string identifier, boost::filesystem::path& path);
    virtual bool SetFilePathForIdentifier(std::string identifier, boost::filesystem::path path);
    
protected:
    PreferenceWriter(std::string prefix) : prefix(prefix) {};
    ~PreferenceWriter() {};
    
    std::string prefix;
};



#endif /* PreferenceRetriever_hpp */

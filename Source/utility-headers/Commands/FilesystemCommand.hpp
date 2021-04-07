//
//  FilesystemCommand.hpp
//  SafeguardTools
//
//  Created by T431962 on 3/10/17.
//
//

#ifndef FilesystemCommand_hpp
#define FilesystemCommand_hpp

#include "IBtCommand.hpp"
#include "FilesystemResults.hpp"
#include <boost/system/system_error.hpp>
#include <boost/filesystem.hpp>

class FilesystemCommand : public VerifySuccessCommand
{
protected:
    FilesystemCommand(boost::filesystem::path path, bool requireSuccess, FilesystemResults& results) : VerifySuccessCommand(requireSuccess), path(path), results(results) {};
    ~FilesystemCommand() {};
    
    boost::filesystem::path path;
    FilesystemResults& results;
};

class CreatePathCommand : public FilesystemCommand
{
public:
    CreatePathCommand(boost::filesystem::path path, bool requireSuccess, FilesystemResults& results) : FilesystemCommand(path, requireSuccess, results) {};
    
    bool Execute() const override;
};

class DeleteFilesMatchingCommand : public FilesystemCommand
{
public:
    DeleteFilesMatchingCommand(boost::filesystem::path path, string matchString, bool requireSuccess, FilesystemResults& results) : FilesystemCommand(path, requireSuccess, results), matchString(matchString) {};
    
    bool Execute() const override;
    
private:
    string matchString;
};

#endif /* FilesystemCommand_hpp */

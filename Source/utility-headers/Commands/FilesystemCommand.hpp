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

class FilesystemCommand : public VerifySuccessCommand
{
protected:
    FilesystemCommand(ai::FilePath path, bool requireSuccess, FilesystemResults& results) : VerifySuccessCommand(requireSuccess), path(path), results(results) {};
    ~FilesystemCommand() {};
    
    ai::FilePath path;
    FilesystemResults& results;
};

class CreatePathCommand : public FilesystemCommand
{
public:
    CreatePathCommand(ai::FilePath path, bool requireSuccess, FilesystemResults& results) : FilesystemCommand(path, requireSuccess, results) {};
    
    bool Execute() const override;
};

class DeleteFilesMatchingCommand : public FilesystemCommand
{
public:
    DeleteFilesMatchingCommand(ai::FilePath path, string matchString, bool requireSuccess, FilesystemResults& results) : FilesystemCommand(path, requireSuccess, results), matchString(matchString) {};
    
    bool Execute() const override;
    
private:
    string matchString;
};

#endif /* FilesystemCommand_hpp */

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

class FilesystemCommand : public IBtCommand
{
protected:
    FilesystemCommand(ai::FilePath path) : path(path) {};
    ~FilesystemCommand() {};
    
    ai::FilePath path;
};

class CreatePathCommand : public FilesystemCommand
{
public:
    CreatePathCommand(ai::FilePath path) : FilesystemCommand(path) {};
    
    bool Execute() const override;
};

#endif /* FilesystemCommand_hpp */

//
//  FilesystemCommand.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/10/17.
//
//

#include "FilesystemCommand.hpp"
#include "PathCreator.h"

bool CreatePathCommand::Execute() const
{
    return PathCreator().CreatePath(path);
}

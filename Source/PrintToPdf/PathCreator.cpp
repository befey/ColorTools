//
//  PathCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/30/16.
//
//

#include "PathCreator.h"

#include <boost/system/system_error.hpp>
#include <boost/filesystem.hpp>

using PrintToPdf::PathCreator;
namespace fs = boost::filesystem;

bool PathCreator::CreatePath(ai::FilePath path) const
{
    fs::path p = path.GetFullPath().as_Platform();
    boost::system::error_code e;
        
    return fs::create_directories(p, e);;
}
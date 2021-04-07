//
//  PathCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/30/16.
//
//

#include "PathCreator.h"

#include <boost/system/system_error.hpp>

namespace fs = boost::filesystem;

bool PathCreator::CreatePath(fs::path path) const
{
    if (fs::exists(path))
    {
        return true;
    }
    else
    {
        return fs::create_directories(path);
    }
}

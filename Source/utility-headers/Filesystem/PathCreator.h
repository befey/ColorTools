//
//  PathCreator.h
//  SafeguardTools
//
//  Created by T431962 on 6/30/16.
//
//

#ifndef __SafeguardTools__PathCreator__
#define __SafeguardTools__PathCreator__

#include <boost/filesystem.hpp>

class PathCreator
{
public:
    bool CreatePath(boost::filesystem::path path) const;
};

#endif /* defined(__SafeguardTools__PathCreator__) */

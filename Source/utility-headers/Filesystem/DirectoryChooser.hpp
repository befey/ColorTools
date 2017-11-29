//
//  DirectoryChooser.hpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/29/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#ifndef DirectoryChooser_h
#define DirectoryChooser_h

#include <boost/filesystem.hpp>

class DirectoryChooser
{
public:
    virtual boost::filesystem::path GetDirectory();
    
protected:
    DirectoryChooser() {};
    ~DirectoryChooser() {};
};


#endif /* DirectoryChooser_h */

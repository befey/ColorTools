//
//  CommandLineDirectoryChooser.hpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/29/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#ifndef CommandLineDirectoryChooser_h
#define CommandLineDirectoryChooser_h

#include "DirectoryChooser.hpp"
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

class CommandLineDirectoryChooser : public DirectoryChooser
{
public:
    CommandLineDirectoryChooser() {};
    
    boost::filesystem::path GetDirectory() override
    {
        std::string strpath;
        std::cout << "/nPlease enter a directory path: ";
        std::cin >> strpath;
        
        return boost::filesystem::path(strpath);
    };
};


#endif /* CommandLineDirectoryChooser_h */

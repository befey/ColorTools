//
//  AiDirectoryChooser.hpp
//  ManufacturingVoidCleanup
//
//  Created by T431962 on 11/29/17.
//  Copyright © 2017 T431962. All rights reserved.
//

#ifndef AiDirectoryChooser_h
#define AiDirectoryChooser_h

#include "DirectoryChooser.hpp"
#include <boost/filesystem.hpp>
#include <string>

#include "AIFilePath.h"
#include "AIUser.h"

extern AIUserSuite* sAIUser;

class AiDirectoryChooser : public DirectoryChooser
{
public:
    AiDirectoryChooser() {};
    
    boost::filesystem::path GetDirectory() override
    {
        std::string strpath;
        
        ai::FilePath saveasFilePath;
        sAIUser->GetDirectoryDialog(ai::UnicodeString("Choose a location to save the PDF files"), saveasFilePath);
        
        return boost::filesystem::path(saveasFilePath.GetFullPath().as_Platform());
    };
};


#endif /* AiDirectoryChooser_h */

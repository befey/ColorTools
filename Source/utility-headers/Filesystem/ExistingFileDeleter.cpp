//
//  ExistingFileDeleter.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/6/16.
//
//

#include "ExistingFileDeleter.h"
#include <boost/system/system_error.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

using Transaction = FilesystemResults::Transaction;

namespace fs = boost::filesystem;

std::unique_ptr<ExistingFileDeleter> ExistingFileDeleter::GetExistingFileDeleter(bool doNotDelete)
{
    return std::unique_ptr<ExistingFileDeleter> { std::make_unique<ExistingFileDeleter>(doNotDelete) };
}

FilesystemResults ExistingFileDeleter::Delete(std::string matchString, fs::path fp)
{
    std::vector<fs::path> filesToDelete;
    FilesystemResults deletedFiles;
    
    if (doNotDelete)
    {
        return deletedFiles;
    }
    
    boost::algorithm::to_upper(matchString);

    typedef std::function<bool(const fs::directory_entry&)> filterfunc;
    filterfunc pred = [matchString](const fs::directory_entry& e)
    {
        std::string currFile = e.path().filename().string();
        boost::algorithm::to_upper(currFile);
        
        if (currFile.find(matchString) != std::string::npos)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    std::copy(
              boost::make_filter_iterator(pred, fs::directory_iterator(fp), fs::directory_iterator()),
              boost::make_filter_iterator(pred, fs::directory_iterator(), fs::directory_iterator()),
              std::back_inserter(filesToDelete)
             );
    
    for (auto fp : filesToDelete)
    {
        boost::system::error_code ec;
        fs::remove(fp, ec);
        if (ec == boost::system::errc::success)
        {
            deletedFiles.AddResult({.action = Transaction::Deleted, .path = fp });
        }
    }
    
    return deletedFiles;
}

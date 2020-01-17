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

    try {
        std::copy(
                  boost::make_filter_iterator(pred, fs::directory_iterator(fp), fs::directory_iterator()),
                  boost::make_filter_iterator(pred, fs::directory_iterator(), fs::directory_iterator()),
                  std::back_inserter(filesToDelete)
                  );
    }
    catch(const fs::filesystem_error& e)
    {
        e.what();
    }
    
    for (auto fp : filesToDelete)
    {
        deletedFiles.AddResult({.action = Transaction::Found, .path = fp });
        boost::system::error_code ec;
        
        if (!doNotDelete)
        {
            try {
                fs::remove(fp);//, ec);
            } catch (boost::filesystem::filesystem_error e ) {
                if ( e.code() == boost::system::errc::device_or_resource_busy )
                {
                    ai::UnicodeString errorMsg = ai::UnicodeString(fp.string() + "\n\nThis file is in use and could not be deleted." );
                    sAIUser->ErrorAlert(errorMsg);
                }
                continue;
            }
            
            if (ec == boost::system::errc::success)
            {
                deletedFiles.AddResult({.action = Transaction::Deleted, .path = fp });
            }
        }
    }
    
    return deletedFiles;
}

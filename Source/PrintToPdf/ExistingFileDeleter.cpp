//
//  ExistingFileDeleter.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/6/16.
//
//

#include "ExistingFileDeleter.h"
#include <boost/system/system_error.hpp>
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

using SafeguardFile::PlateNumber;
using PrintToPdf::ExistingFileDeleter;
using PrintToPdf::PdfResults;
using Transaction = PrintToPdf::PdfResults::Transaction;

namespace fs = boost::filesystem;

PdfResults ExistingFileDeleter::Delete(PlateNumber pn, ai::FilePath fp)
{
    fs::path p = fp.GetFullPath().as_Platform();
    string filename = pn;
    boost::algorithm::to_upper(filename);
    
    vector<fs::path> filesToDelete;
    PdfResults deletedFiles;

    typedef function<bool(const fs::directory_entry&)> filterfunc;
    filterfunc pred = [filename](const fs::directory_entry& e)
    {
        string currFile = e.path().filename().string();
        boost::algorithm::to_upper(currFile);
        
        if (currFile.find(filename) != string::npos)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    std::copy(
              boost::make_filter_iterator(pred, fs::recursive_directory_iterator(p), fs::recursive_directory_iterator()),
              boost::make_filter_iterator(pred, fs::recursive_directory_iterator(), fs::recursive_directory_iterator()),
              std::back_inserter(filesToDelete)
             );
    
    for (auto p : filesToDelete)
    {
        boost::system::error_code ec;
        fs::remove(p, ec);
        if (ec == boost::system::errc::success)
        {
            deletedFiles.AddResult({.action = Transaction::Deleted, .path = ai::FilePath(ai::UnicodeString(p.string())) });
        }
    }
    
    return deletedFiles;
}
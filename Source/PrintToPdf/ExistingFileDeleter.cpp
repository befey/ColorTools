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
#include <iostream>
using PrintToPdf::ExistingFileDeleter;
using PrintToPdf::PdfResults;
namespace fs = boost::filesystem;

PdfResults ExistingFileDeleter::Delete(PlateNumber pn, ai::FilePath fp)
{
    fs::path p = fp.GetFullPath().as_Platform();
    
    /*const fs::recursive_directory_iterator end;
    const auto it = find_if(fs::recursive_directory_iterator(p), end,
                            [](const fs::directory_entry& e)
                            {
                                return e.path().filename() == file_name;
                            });
    */
    /*boost::filter_iterator<function<fs::directory_entry&>, fs::recursive_directory_iterator>(
                                                                                             [](const fs::directory_entry& e){return e;},
                                                                                             fs::recursive_directory_iterator(p),
    
                                                                                             fs::recursive_directory_iterator());
    */
    
    auto pred = [](const fs::directory_entry& e){return true;};
    std::copy(
              boost::make_filter_iterator(pred, fs::recursive_directory_iterator(p), fs::recursive_directory_iterator()),
              boost::make_filter_iterator(pred, fs::recursive_directory_iterator(), fs::recursive_directory_iterator()),
              std::ostream_iterator<fs::directory_entry&>(std::cout, "\n")
             );
    return PdfResults();
}
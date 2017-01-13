//
//  PlaceFileSearcher.cpp
//  SafeguardTools
//
//  Created by T431962 on 1/12/17.
//
//

#include "PlaceFileSearcher.hpp"
#include <boost/system/system_error.hpp>
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/algorithm/string.hpp>
#include "PathBuilder.h"

using PlaceFileSearch::PlaceFileSearcher;
using PlaceFileSearch::PlaceFileSearchResults;
using PrintToPdf::PathBuilder;
using PrintToPdf::PdfPreset;
using SafeguardFile::PlateNumber;

namespace fs = boost::filesystem;

PlaceFileSearchResults PlaceFileSearcher::DoSearch() const
{
    unique_ptr<PathBuilder> pBuilder = PathBuilder::GetPathBuilder(PdfPreset::Manufacturing, false);
    ai::FilePath fp = pBuilder->GetAiFilePath(PlateNumber(searchString));
    
    vector<fs::path> foundFilePaths;
    
    fs::path p = fp.GetFullPath().as_Platform();
    
    string searchSt = searchString;
    boost::algorithm::to_upper(searchSt);
    
    typedef function<bool(const fs::directory_entry&)> filterfunc;
    filterfunc pred = [searchSt](const fs::directory_entry& e)
    {
        string currFile = e.path().filename().string();
        boost::algorithm::to_upper(currFile);
        
        if (currFile.find(searchSt) != string::npos)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    
    std::copy(
              boost::make_filter_iterator(pred, fs::directory_iterator(p), fs::directory_iterator()),
              boost::make_filter_iterator(pred, fs::directory_iterator(), fs::directory_iterator()),
              std::back_inserter(foundFilePaths)
              );
    
    vector<string> results;
    for (auto p : foundFilePaths)
    {
        results.push_back(p.string());
    }
    
    
    return PlaceFileSearchResults(results);
}


string PlaceFileSearchResults::GetResultsAsXML()
{
    //Format as XML string
    string xmlString;
    
    xmlString.append("<root>");
    for (auto it : results)
    {
        xmlString.append("<path>").append(it).append("</path>");
    }
    xmlString.append("</root>");
    
    return xmlString;
}

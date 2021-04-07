//
//  PlaceFileSearcher.hpp
//  SafeguardTools
//
//  Created by T431962 on 1/12/17.
//
//

#ifndef PlaceFileSearcher_hpp
#define PlaceFileSearcher_hpp

#include <vector>
#include <string>

namespace PlaceFileSearch
{
    static constexpr auto PLACEFILESEARCH_FOLDERPREFS_EXTENSION =  "com.gosafeguard.SafeguardTools.PlaceFileSearch.FolderPrefs";
    
    class PlaceFileSearchResults
    {
    public:
        PlaceFileSearchResults(vector<string> results) : results(results) {};
        string GetResultsAsXML();
    private:
        vector<string> results;
    };
    
    class PlaceFileSearcher
    {
    public:
        PlaceFileSearcher(string searchString) : searchString(searchString) {};
        PlaceFileSearchResults DoSearch() const;
    private:
        string searchString;
    };
}

#endif /* PlaceFileSearcher_hpp */

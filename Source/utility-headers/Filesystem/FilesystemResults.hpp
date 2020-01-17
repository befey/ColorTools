//
//  FilesystemResults.hpp
//  SafeguardTools
//
//  Created by T431962 on 3/10/17.
//
//

#ifndef FilesystemResults_hpp
#define FilesystemResults_hpp

#include <string>
#include <vector>
#include <set>
#include "PlateNumber.h"
#include <boost/filesystem.hpp>

class FilesystemResults
{
public:
    struct Transaction
    {
        enum Action
        {
            Deleted,
            Created,
            Found
        };
        Action action;
        boost::filesystem::path path;
    };
    
    void AddResult(Transaction transaction);
    void AddResult(FilesystemResults resultList);
    
    std::string MakeXmlString() const;
    
    bool IsTransactionFound()
    {
        for (auto r : results)
        {
            if (r.action == Transaction::Found)
                return true;
        }
        return false;
    };
    
    std::vector<std::string> GetAsVectorOfPlateNumbers() const
    {
        std::set<std::string> list;
        for ( auto r : results )
        {
            std::string fn = r.path.filename().string();
            std::string strippedPlateNumber = boost::filesystem::change_extension(fn, "").string();
            list.insert(SafeguardFile::PlateNumber(strippedPlateNumber));
        }
        std::vector<std::string> vstring;
        for (auto l : list)
        {
            vstring.push_back(l);
        }
        return vstring;
    };
private:
    std::vector<Transaction> results;
};

#endif /* FilesystemResults_hpp */

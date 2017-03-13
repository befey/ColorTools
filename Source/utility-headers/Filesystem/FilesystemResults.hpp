//
//  FilesystemResults.hpp
//  SafeguardTools
//
//  Created by T431962 on 3/10/17.
//
//

#ifndef FilesystemResults_hpp
#define FilesystemResults_hpp

class FilesystemResults
{
public:
    struct Transaction
    {
        enum Action
        {
            Deleted,
            Created
        };
        Action action;
        ai::FilePath path;
    };
    
    void AddResult(Transaction transaction);
    void AddResult(FilesystemResults resultList);
    
    string MakeXmlString() const;
    
private:
    std::vector<Transaction> results;
};

#endif /* FilesystemResults_hpp */

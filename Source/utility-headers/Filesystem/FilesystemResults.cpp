//
//  FilesystemResults.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/10/17.
//
//

#include "FilesystemResults.hpp"

void FilesystemResults::AddResult(Transaction transaction)
{
    results.push_back(transaction);
}

void FilesystemResults::AddResult(FilesystemResults resultList)
{
    for (auto transaction : resultList.results)
    {
        AddResult(transaction);
    }
}

std::string FilesystemResults::MakeXmlString() const
{
    //Format as XML string
    std::string xmlString;
    
    xmlString.append("<root>");
    for (auto it : results)
    {
        if (it.action == Transaction::Action::Created)
        {
            xmlString.append("<create>").append(it.path.string()).append("</create>");
        }
        else if (it.action == Transaction::Action::Deleted)
        {
            xmlString.append("<delete>").append(it.path.string()).append("</delete>");
        }
    }
    xmlString.append("</root>");
    
    return xmlString;
}

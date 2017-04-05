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

string FilesystemResults::MakeXmlString() const
{
    //Format as XML string
    string xmlString;
    
    xmlString.append("<root>");
    for (auto it : results)
    {
        if (it.action == Transaction::Action::Created)
        {
            xmlString.append("<create>").append(it.path.GetFullPath().getInStdString(kAIPlatformCharacterEncoding)).append("</create>");
        }
        else if (it.action == Transaction::Action::Deleted)
        {
            xmlString.append("<delete>").append(it.path.GetFullPath().getInStdString(kAIPlatformCharacterEncoding)).append("</delete>");
        }
    }
    xmlString.append("</root>");
    
    return xmlString;
}

//
//  PdfResults.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/27/16.
//
//

#include "PdfResults.h"
using PrintToPdf::PdfResults;

void PdfResults::AddResult(Transaction transaction)
{
    results.push_back(transaction);
}

string PdfResults::MakeXmlString() const
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

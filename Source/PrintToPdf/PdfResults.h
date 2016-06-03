//
//  PdfResults.h
//  SafeguardTools
//
//  Created by T431962 on 4/27/16.
//
//

#ifndef __SafeguardTools__PdfResults__
#define __SafeguardTools__PdfResults__

namespace PrintToPdf
{
    class PdfResults
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
        
        std::vector<Transaction> results;
        
        void AddResult(Transaction);
        string MakeXmlString() const;
    };
}

#endif /* defined(__SafeguardTools__PdfResults__) */

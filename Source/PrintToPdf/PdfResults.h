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
        
        void AddResult(Transaction);
        void AddResult(PdfResults);
        
        string MakeXmlString() const;
        
    private:
        std::vector<Transaction> results;
    };
}

#endif /* defined(__SafeguardTools__PdfResults__) */

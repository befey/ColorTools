//
//  SafeguardJobFile.h
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#ifndef __SafeguardTools__SafeguardJobFile__
#define __SafeguardTools__SafeguardJobFile__

#include <vector>
#include "Plate.h"
#include "PlateNumber.h"
#include "TickMarkDrawer.h"
#include "PdfPrinter.h"
#include "PdfResults.h"
#include "PrintToPdfConstants.h"

namespace SafeguardFile
{
    class SafeguardJobFile
    {
    public:
        void Update();
        void RemoveBleedInfo();
        PrintToPdf::PdfResults Print() const;
        
        void SetPdfPrinter(unique_ptr<PrintToPdf::PdfPrinter> printer) { pdfPrinter = move(printer); };
        const PlateNumber GetPlateNumber() const;
        const string GetToken(int plateIndex) const;
        AIRealRect GetBleeds(PrintToPdf::PdfPreset preset) const;
    private:
        vector<Plate> plates;
        unique_ptr<PrintToPdf::PdfPrinter> pdfPrinter;
        
        void AddBleedInfo();
        bool ShouldDrawBleedInfo();
    };
}
#endif /* defined(__SafeguardTools__SafeguardJobFile__) */

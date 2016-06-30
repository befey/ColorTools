//
//  SafeguardJobFile.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "SafeguardJobFile.h"
#include "DictFuncs.h"

using SafeguardFile::SafeguardJobFile;

void SafeguardJobFile::Update()
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardID count;
    sAIArtboard->GetCount(abList, count);
    
    for (int i = 0; i < count; i++)
    {
        plates.push_back(Plate(i));
    }
    
    if (ShouldDrawBleedInfo()) {
        AddBleedInfo();
    }
}


void SafeguardJobFile::AddBleedInfo()
{
    if (ShouldDrawBleedInfo())
    {
        for (auto plate : plates)
        {
            plate.AddBleedInfo();
        }
    }
}

void SafeguardJobFile::RemoveBleedInfo()
{
    for (auto plate : plates)
    {
        plate.RemoveBleedInfo();
    }
}

bool SafeguardJobFile::ShouldDrawBleedInfo()
{
    //if ( CheckDictionaryForArtObjectWithIdentifier(ai::UnicodeString(SafeguardFile::PLATE_BLEED_INFO_GROUP_LABEL), 0) )
    //{
    //    return true;
    //}
    return false;
}

PrintToPdf::PdfResults SafeguardJobFile::Print() const
{
    if (pdfPrinter == nullptr)
    {
        return PrintToPdf::PdfResults();
    }
    return pdfPrinter->Print();
}

const PlateNumber SafeguardJobFile::GetPlateNumber() const
{
    if (!plates.empty())
    {
        return plates[0].GetPlateNumber();
    }
    return PlateNumber();
}

const string SafeguardJobFile::GetToken(int plateIndex) const
{
    if (plates.size() > plateIndex)
    {
        return plates[plateIndex].GetToken();
    }
    return "";
}

AIRealRect SafeguardJobFile::GetBleeds(PrintToPdf::PdfPreset preset) const
{
    if (!plates.empty())
    {
        return plates[0].GetBleeds(preset);
    }
    return AIRealRect{0,0,0,0};
}
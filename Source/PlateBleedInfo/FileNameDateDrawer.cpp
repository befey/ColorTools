//
//  FileNameDateDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#include "FileNameDateDrawer.h"
#include "ColorFuncs.h"
#include "BleedInfo.h"
#include <ctime>

using SafeguardFile::FileNameDateDrawer;
using SafeguardFile::LaserFileNameDateDrawer;
using SafeguardFile::ContinuousFileNameDateDrawer;
using SafeguardFile::BusStatFileNameDateDrawer;
using SafeguardFile::BleedInfo;

FileNameDateDrawer::FileNameDateDrawer(shared_ptr<BleedInfo> info) : BleedTextInfoDrawer(info) {};
LaserFileNameDateDrawer::LaserFileNameDateDrawer(shared_ptr<BleedInfo> info) : FileNameDateDrawer(info) {};
ContinuousFileNameDateDrawer::ContinuousFileNameDateDrawer(shared_ptr<BleedInfo> info) : FileNameDateDrawer(info) {};
BusStatFileNameDateDrawer::BusStatFileNameDateDrawer(shared_ptr<BleedInfo> info) : FileNameDateDrawer(info) {};

AIArtHandle LaserFileNameDateDrawer::Draw()
{
    AIArtHandle plateNumberDateArt;
    AIRealPoint anchor = {.h = p_BleedInfo->rect.right - 4, .v = p_BleedInfo->rect.bottom - 14};
    sAITextFrame->NewPointText(kPlaceAboveAll, NULL, kHorizontalTextOrientation, anchor, &plateNumberDateArt);
    
    //Create the ATE range
    ATE::TextRangeRef plateInfoTextRangeRef;
    sAITextFrame->GetATETextRange(plateNumberDateArt, &plateInfoTextRangeRef);
    ATE::ITextRange plateInfoTextRange(plateInfoTextRangeRef);
    plateInfoTextRange.Remove();
    
    p_BleedInfo->plateNumber.GetAsTextRange(plateInfoTextRange);
    
    if (p_BleedInfo->token != "")
    {
        AddTextToRange("." + p_BleedInfo->token, plateInfoTextRange);
    }
    
    int month, year;
    month = p_BleedInfo->lastModified.tm_mon + 1;
    year = p_BleedInfo->lastModified.tm_year + 1900;
    
    AddTextToRange("  " + to_string(month) + "/" + to_string(year), plateInfoTextRange);

    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(12.01).Font("Helvetica-Bold").Justification(ATE::kRightJustify).FillColor(GetRegistrationColor());
    textFeatures.ApplyFeaturesToRange(plateInfoTextRange);
    
    return plateNumberDateArt;
}
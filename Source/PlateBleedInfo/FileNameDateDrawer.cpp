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

FileNameDateDrawer::FileNameDateDrawer(AIRealRect bounds, AIRealPoint anchor, PlateNumber plateNumber, string token, tm lastModified) :
    BleedTextInfoDrawer(bounds, anchor),
    plateNumber(plateNumber),
    token(token),
    lastModified(lastModified) {};

LaserFileNameDateDrawer::LaserFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified) : FileNameDateDrawer(bounds, {.h = bounds.right - 4, .v = bounds.bottom - 14}, plateNumber, token, lastModified) {};
ContinuousFileNameDateDrawer::ContinuousFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified) : FileNameDateDrawer(bounds, {.h = bounds.right - 4, .v = bounds.bottom - 14}, plateNumber, token, lastModified) {};
BusStatFileNameDateDrawer::BusStatFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified) : FileNameDateDrawer(bounds, {.h = bounds.right - 4, .v = bounds.bottom - 14}, plateNumber, token, lastModified) {};

AIArtHandle LaserFileNameDateDrawer::DoDraw() const
{
    AIArtHandle plateNumberDateArt;
    sAITextFrame->NewPointText(kPlaceAboveAll, NULL, kHorizontalTextOrientation, anchor, &plateNumberDateArt);
    
    //Create the ATE range
    ATE::TextRangeRef plateInfoTextRangeRef;
    sAITextFrame->GetATETextRange(plateNumberDateArt, &plateInfoTextRangeRef);
    ATE::ITextRange plateInfoTextRange(plateInfoTextRangeRef);
    plateInfoTextRange.Remove();
    
    plateNumber.GetAsTextRange(plateInfoTextRange);
    
    if (token != "")
    {
        AddTextToRange("." + token, plateInfoTextRange);
    }
    
    int month, year;
    month = lastModified.tm_mon + 1;
    year = lastModified.tm_year + 1900;
    
    AddTextToRange("  " + to_string(month) + "/" + to_string(year), plateInfoTextRange);

    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(12.01).Font("Helvetica-Bold").Justification(ATE::kRightJustify).FillColor(GetRegistrationColor());
    textFeatures.ApplyFeaturesToRange(plateInfoTextRange);
    
    return plateNumberDateArt;
}
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
#include "BtTransformArt.hpp"
#include "BtLayer.hpp"
#include "SafeguardFileConstants.h"
#include <ctime>

using SafeguardFile::FileNameDateDrawer;
using SafeguardFile::LaserFileNameDateDrawer;
using SafeguardFile::ContinuousFileNameDateDrawer;
using SafeguardFile::BusStatFileNameDateDrawer;

FileNameDateDrawer::FileNameDateDrawer(AIRealRect bounds, AIRealPoint anchor, PlateNumber plateNumber, string token, tm lastModified) :
    BleedTextInfoDrawer(bounds, anchor),
    plateNumber(plateNumber),
    token(token),
    lastModified(lastModified)
{
    maxWidth = (artboardBounds.right - artboardBounds.left);
    maxHeight = (artboardBounds.top - artboardBounds.bottom);
}

LaserFileNameDateDrawer::LaserFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified) : FileNameDateDrawer(bounds, {.h = bounds.right - 4, .v = bounds.bottom - 4.5}, plateNumber, token, lastModified) {};
ContinuousFileNameDateDrawer::ContinuousFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified) : FileNameDateDrawer(bounds, {.h = bounds.right + 2, .v = bounds.top - 14}, plateNumber, token, lastModified) {};
BusStatFileNameDateDrawer::BusStatFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified) : FileNameDateDrawer(bounds, {.h = bounds.right, .v = bounds.bottom - 4.5}, plateNumber, token, lastModified) {};

AIArtHandle LaserFileNameDateDrawer::Draw(AIArtHandle resultGroup) const
{
    AIArtHandle plateNumberDateArt;
    
    ATE::ITextRange range = SetupTextRange(resultGroup, maxWidth, ATE::kRightJustify, kHorizontalTextOrientation, &plateNumberDateArt);
    
    PutPlateNumberDateStringInTextRange(range);
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(12).Font("Helvetica-Bold").Justification(ATE::kRightJustify).FillColor(GetRegistrationColor());
    textFeatures.ApplyFeaturesToRange(range);
    
    FitTextFrameToContents(plateNumberDateArt);
    
    return plateNumberDateArt;
}

AIArtHandle ContinuousFileNameDateDrawer::Draw(AIArtHandle resultGroup) const
{
    AIArtHandle plateNumberDateArt;
    
    //We pass maxHeight here since we're rotating
    ATE::ITextRange range = SetupTextRange(resultGroup, maxHeight, ATE::kLeftJustify, kHorizontalTextOrientation, &plateNumberDateArt);
    
    PutPlateNumberDateStringInTextRange(range);
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(8).Font("Helvetica").Justification(ATE::kLeftJustify).FillColor(GetRegistrationColor());
    textFeatures.ApplyFeaturesToRange(range);

    FitTextFrameToContents(plateNumberDateArt);
    
    RotateArt(plateNumberDateArt, anchor, -90);
    MoveArtOutsideBounds(plateNumberDateArt, artboardBounds, Direction::Right, 0);
    
    return plateNumberDateArt;
}

AIArtHandle BusStatFileNameDateDrawer::Draw(AIArtHandle resultGroup) const
{
    AIArtHandle plateNumberDateArt;
    
    ATE::ITextRange range = SetupTextRange(resultGroup, maxWidth, ATE::kRightJustify, kHorizontalTextOrientation, &plateNumberDateArt);
    
    PutPlateNumberDateStringInTextRange(range);
    BtAteTextFeatures textFeatures;
    textFeatures.FontSize(7).Font("Helvetica-Condensed-Bold").Justification(ATE::kRightJustify).FillColor(GetRegistrationColor());
    textFeatures.ApplyFeaturesToRange(range);
    
    FitTextFrameToContents(plateNumberDateArt);
    
    return plateNumberDateArt;
}


void FileNameDateDrawer::PutPlateNumberDateStringInTextRange(ATE::ITextRange& targetRange) const
{
    plateNumber.AsTextRange(targetRange);
    
    if (token != "")
    {
        AddTextToRange("." + token, targetRange);
    }
    
    int month, year;
    month = lastModified.tm_mon + 1;
    year = lastModified.tm_year + 1900;
    
    AddTextToRange("  " + to_string(month) + "/" + to_string(year), targetRange);
}

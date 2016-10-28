//
//  BleedInfo.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/26/16.
//
//

#include "BleedInfo.h"
#include "ArtTree.h"
#include <boost/system/system_error.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

using SafeguardFile::BleedInfo;

BleedInfo::BleedInfo(ai::ArtboardID artboardIndex)
: artboardIndex(artboardIndex)
{
    FillColorList();
    colorList.RemoveDuplicates();
    colorList.RemoveNonPrintingColors();
    colorList.Sort();

    SetPlateNumber();
    
    tmSettings = SafeguardFile::TickMarkSettings(ArtboardBounds(), plateNumber.GetProductType(), SafeguardFile::TickMarkStyle::NONE);
}

AIRealRect BleedInfo::ArtboardBounds() const
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties props;
    sAIArtboard->GetArtboardProperties(abList, artboardIndex, props);
    AIRealRect rect;
    props.GetPosition(rect);
    return rect;
}

tm BleedInfo::LastModified() const
{
    ai::FilePath aiFilePath;
    sAIDocument->GetDocumentFileSpecification(aiFilePath);
    
    boost::system::error_code ec;
    time_t lastWrite = fs::last_write_time(aiFilePath.GetFullPath().as_Platform(), ec);
    
    if (ec != boost::system::errc::success)
    {
        std::time(&lastWrite);
    }
    
    return *localtime(&lastWrite);
}

string BleedInfo::Token() const
{
    bool isDefaultName;
    string abNameS = ArtboardName(isDefaultName);
    
    if (isDefaultName || abNameS == NO_TOKEN_DESIG)
    {
        return "";
    }
    else
    {
        return abNameS;
    }
}

BleedInfo& BleedInfo::Token(string newVal)
{
    if (newVal == "")
    {
        newVal = NO_TOKEN_DESIG;
    }
    return ArtboardName(newVal);
}


string BleedInfo::ArtboardName(bool& isDefault) const
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties props;
    sAIArtboard->GetArtboardProperties(abList, ArtboardIndex(), props);
    ai::UnicodeString abName;
    props.GetName(abName);
    string abNameS = abName.getInStdString(kAIPlatformCharacterEncoding);
    
    AIBoolean aiboolIsDefault;
    sAIArtboard->IsDefaultName(props, aiboolIsDefault);
    isDefault = aiboolIsDefault;
    
    return abNameS;
}

string BleedInfo::ArtboardName() const
{
    bool aiboolIsDefault;
    return ArtboardName(aiboolIsDefault);
}

BleedInfo& BleedInfo::ArtboardName(string newVal)
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties props;
    sAIArtboard->GetArtboardProperties(abList, ArtboardIndex(), props);
    ai::UnicodeString oldNameUS;
    props.GetName(oldNameUS);
    if (oldNameUS.as_Platform() != newVal)
    {
        props.SetName(ai::UnicodeString(newVal));
        sAIArtboard->Update(abList, ArtboardIndex(), props);
    }
    
    return *this;
}

void BleedInfo::FillColorList()
{
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    CreateArtSetOfPrintingObjectsWithinRect(artSet, ArtboardBounds());
    
    std::function<void(AIArtHandle)> func = std::bind(&BleedInfo::AddColorsOfArtToColorList, this, std::placeholders::_1);
    ProcessArtSet(artSet, func);
    sAIArtSet->DisposeArtSet(&artSet);
}

void BleedInfo::AddColorsOfArtToColorList(AIArtHandle art)
{
    colorList.AddColorsToList(GetColorsFromArt(art));
}

void BleedInfo::SetPlateNumber()
{
    //TODO: Make this handle the other plate number cases when we don't want to use the filename
    ai::FilePath openedFilePath;
    sAIDocument->GetDocumentFileSpecification(openedFilePath);
    SetPlateNumber(openedFilePath.GetFileNameNoExt().getInStdString(kAIPlatformCharacterEncoding));
}

void BleedInfo::SetPlateNumber(string pn)
{
    plateNumber = SafeguardFile::PlateNumber(pn);
}

AIRealRect BleedInfo::Bleeds() const
{
    AIRealRect bleedRect;
    
    ProductType pt = PlateNumber().GetProductType();
    
    if (pt == ProductType::CutSheet)
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 36, 36, 36, 36);
    }
    else if (pt == ProductType::BusinessStat)
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 12, 12, 12, 12);
    }
    else if (pt == ProductType::Continuous)
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 9, 9, 9, 9);
    }
    else //No bleed
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 0, 0, 0, 0);
    }
    
    return bleedRect;
}

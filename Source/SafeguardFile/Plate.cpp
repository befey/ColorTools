//
//  Plate.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "Plate.h"
#include "SafeguardFileConstants.h"
#include "BtAteTextFeatures.h"
#include "TickMarkDrawer.h"
#include "TickMarkSettings.hpp"
#include "ColorListDrawer.h"
#include "FileNameDateDrawer.h"
#include "ATEFuncs.h"
#include "ColorFuncs.h"
#include "ArtTree.h"
#include "AIColor.h"
#include "DictionaryWriter.h"
#include <functional>
#include <boost/filesystem/operations.hpp>

using SafeguardFile::Plate;
using SafeguardFile::PlateNumber;
using PrintToPdf::PdfPreset;
using SafeguardFile::BleedInfo;

namespace fs = boost::filesystem;

Plate::Plate(ai::ArtboardID id)
{
    bleedInfo.artboardIndex = id;
    bleedInfo.rect = GetArtboardBounds();
    bleedInfo.ShouldAddCMYKBlocks = true;
    bleedInfo.lastModified = GetLastModified();
    
    FillColorList();
    bleedInfo.colorList.RemoveDuplicates();
    bleedInfo.colorList.RemoveNonPrintingColors();
    SetPlateNumber();
    bleedInfo.token = CreateToken();
    
    bleedInfoDrawer = make_shared<BleedInfoDrawer>(bleedInfo.artboardIndex);
    
    bleedInfoDrawer->AddDrawer( make_shared<TickMarkDrawer>(TickMarkSettings(bleedInfo)) );
    
    ProductType pt = bleedInfo.plateNumber.GetProductType();
    if (pt == ProductType::BusinessStat)
    {
        bleedInfoDrawer->AddDrawer( make_shared<BusStatColorListDrawer>(bleedInfo.rect, bleedInfo.colorList) );
        bleedInfoDrawer->AddDrawer( make_shared<BusStatFileNameDateDrawer>(bleedInfo.rect, bleedInfo.plateNumber, bleedInfo.token, bleedInfo.lastModified) );
    }
    else if (pt == ProductType::Continuous)
    {
        bleedInfoDrawer->AddDrawer( make_shared<ContinuousColorListDrawer>(bleedInfo.rect, bleedInfo.colorList) );
        bleedInfoDrawer->AddDrawer( make_shared<ContinuousFileNameDateDrawer>(bleedInfo.rect, bleedInfo.plateNumber, bleedInfo.token, bleedInfo.lastModified) );
    }
    else if (pt == ProductType::CutSheet || pt == ProductType::Envelope)
    {
        bleedInfoDrawer->AddDrawer( make_shared<LaserColorListDrawer>(bleedInfo.rect, bleedInfo.colorList) );
        bleedInfoDrawer->AddDrawer( make_shared<LaserFileNameDateDrawer>(bleedInfo.rect, bleedInfo.plateNumber, bleedInfo.token, bleedInfo.lastModified) );
    }
}
//Plate::Plate(ai::ArtboardID id, string pn) : artboardIndex(id), plateNumber(pn) {}

void Plate::SetPlateNumber()
{
    //TODO: Make this handle the other plate number cases when we don't want to use the filename
    ai::FilePath openedFilePath;
    sAIDocument->GetDocumentFileSpecification(openedFilePath);
    SetPlateNumber(openedFilePath.GetFileNameNoExt().getInStdString(kAIPlatformCharacterEncoding));    
}

void Plate::SetPlateNumber(string pn)
{
    bleedInfo.plateNumber = PlateNumber(pn);
}

const PlateNumber Plate::GetPlateNumber() const
{
    return bleedInfo.plateNumber;
}

const string Plate::GetToken() const
{
    return CreateToken();
}

const BleedInfo Plate::GetBleedInfo() const
{
    return bleedInfo;
}

void Plate::AddBleedInfo()
{
    bleedInfoDrawer->Draw();
}

void Plate::RemoveBleedInfo()
{
    unique_ptr<DictionaryWriter> dw = make_unique<DictionaryWriter>();
    AIArtHandle foundArt = dw->GetArtHandleFromIdentifier(PLATE_BLEED_INFO_GROUP_LABEL, bleedInfo.artboardIndex);
    if (foundArt)
    {
        bleedInfoDrawer->Remove(foundArt);
    }
}

string Plate::CreateToken() const
{
    AIBoolean isDefaultName;
    string abNameS = GetArtboardName(isDefaultName);
    
    if (isDefaultName || abNameS == NO_TOKEN_DESIG)
    {
        return "";
    }
    else
    {
        return abNameS;
    }
}
                    
AIRealRect Plate::GetArtboardBounds() const
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties props;
    sAIArtboard->GetArtboardProperties(abList, bleedInfo.artboardIndex, props);
    AIRealRect rect;
    props.GetPosition(rect);
    return rect;
}

tm Plate::GetLastModified() const
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

string Plate::GetArtboardName(AIBoolean& isDefault) const
{
    ai::ArtboardList abList;
    sAIArtboard->GetArtboardList(abList);
    ai::ArtboardProperties props;
    sAIArtboard->GetArtboardProperties(abList, bleedInfo.artboardIndex, props);
    ai::UnicodeString abName;
    props.GetName(abName);
    string abNameS = abName.getInStdString(kAIPlatformCharacterEncoding);
    sAIArtboard->IsDefaultName(props, isDefault);
    
    return abNameS;
}

void Plate::FillColorList()
{
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    CreateArtSetOfPrintingObjectsWithinRect(artSet, GetArtboardBounds());
    
    std::function<void(AIArtHandle)> func = std::bind(&Plate::AddColorsOfArtToColorList, this, std::placeholders::_1);
    ProcessArtSet(artSet, func);
}

void Plate::AddColorsOfArtToColorList(AIArtHandle art)
{
    bleedInfo.colorList.AddColorsToList(GetColorsFromArt(art));
}

AIRealRect Plate::GetBleeds() const
{
    AIRealRect bleedRect;
    
    ProductType pt = GetPlateNumber().GetProductType();
    
    if (pt == ProductType::CutSheet)
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 36, 36, 36, 36);
    }
    else if (pt == ProductType::BusinessStat)
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 12, 12, 12, 12);
    }
    else //Continuous and Snapsets
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 0, 0, 0, 0);
    }
    
    return bleedRect;
}

SafeguardFile::ColorList Plate::GetColors() const
{
    return bleedInfo.colorList;
}

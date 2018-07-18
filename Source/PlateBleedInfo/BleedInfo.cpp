     //
//  BleedInfo.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/26/16.
//
//

#include "BleedInfo.h"
#include "BtArtHandle.hpp"
#include "ArtTree.h"
#include "BleedInfoPluginArtToArtboardMatcher.hpp"
#include "DictionaryWriter.h"
#include "BleedInfoWriter.hpp"
#include "BleedInfoDrawableController.h"
#include "SafeguardJobFileDTO.hpp"
#include "SafeguardFileConstants.h"
#include "TokenCreator.hpp"
#include "ArtboardNameRetriever.hpp"
#include "CurrentFilenameRetriever.hpp"
#include <boost/system/system_error.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

using PlateBleedInfo::BleedInfo;

BleedInfo::BleedInfo(ai::ArtboardID artboardIndex, bool redrawAllWithoutCheck, const PlateBleedInfo::PlateDTO* dto)
:
artboardIndex(artboardIndex),
colorList(ArtboardBounds()),
redrawAllWithoutCheck(redrawAllWithoutCheck)
{
    bleedInfoPluginArt = PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().GetArt(artboardIndex);
    
    SetPlateNumber();
    
    SetShouldPrint();
    
    tmSettings = SafeguardFile::TickMarkSettings(ArtboardBounds(), plateNumber.GetProductType(), SafeguardFile::TickMarkStyle::NONE, shouldDrawBleedInfo);
    
    if (dto != nullptr)
    {
        FillBleedInfoFromPlateDTO(dto, true);
    }
    else
    {
        ReadFromPluginArt();
    }
    
    if (plateNumber.GetProductType() == SafeguardFile::CutSheet && colorList.HasCMYK())
    {
        shouldAddCMYKBlocks = true;
    }
}

BleedInfo::~BleedInfo()
{
    StoreInPluginArt();
}

AIRealRect BleedInfo::ArtboardBounds() const
{
    return GetArtboardBounds(artboardIndex);
}

tm BleedInfo::LastModified() const
{
    time_t lastWrite = std::time(nullptr);
    
    AIBoolean modified = false;
    sAIDocument->GetDocumentModified(&modified);
    
    if (!modified)
    {
        ai::FilePath aiFilePath;
        sAIDocument->GetDocumentFileSpecification(aiFilePath);
        
        boost::system::error_code ec;
        lastWrite = fs::last_write_time(aiFilePath.GetFullPath().as_Platform(), ec);
        
        if (ec != boost::system::errc::success)
        {
            lastWrite = std::time(nullptr);
        }
    }
    
    return *localtime(&lastWrite);
}

string BleedInfo::Token() const
{
    return SafeguardFile::TokenCreator(plateNumber, ArtboardName()).GetToken();
}

BleedInfo& BleedInfo::Token(string newVal)
{
    if (newVal == "")
    {
        newVal = SafeguardFile::NO_TOKEN_DESIG;
    }
    return ArtboardName(newVal);
}


string BleedInfo::ArtboardName(bool& isDefault) const
{
    return ArtboardNameRetriever::GetName(ArtboardIndex(), isDefault);
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

void BleedInfo::SetPlateNumber()
{
    //TODO: Make this handle the other plate number cases when we don't want to use the filename
    SetPlateNumber(CurrentFilenameRetriever::GetFilenameNoExt());
}

void BleedInfo::SetPlateNumber(string pn)
{
    plateNumber = SafeguardFile::PlateNumber(pn);
}

void BleedInfo::SetShouldPrint()
{
    string uCaseArtboardName = ArtboardName();
    for (auto & c: uCaseArtboardName) c = toupper(c);
    SafeguardFile::PlateNumber artboardNameAsPlateNumber(uCaseArtboardName);
    
    if ( plateNumber.GetProductIndicator() == "MP" )
    {
        if ( uCaseArtboardName.find((string)plateNumber) == string::npos )
        {
            shouldPrint = false;
        }
    }
    else if ( plateNumber.GetProductIndicator() != "MP" )
    {
        if ( artboardNameAsPlateNumber.IsValid() && artboardNameAsPlateNumber.GetProductIndicator() == "MP" )
        {
            shouldPrint = false;
        }
    }
    
    if (uCaseArtboardName.find("COMPOSITE") != string::npos)
    {
        shouldPrint = false;
    }
}

AIRealRect BleedInfo::Bleeds() const
{
    AIRealRect bleedRect;

    AIRealRect diff = {0,0,0,0};//GetExpansionAmountToContainRect(ArtboardBounds(), BtArtHandle(bleedInfoPluginArt).Bounds());
    
    SafeguardFile::ProductType pt = PlateNumber().GetProductType();
    
    if (pt == SafeguardFile::ProductType::CutSheet)
    {
        if (diff.left < 36) diff.left = 36;
        if (diff.top < 36) diff.top = 36;
        if (diff.right < 36) diff.right = 36;
        if (diff.bottom < 36) diff.bottom = 36;
        sAIRealMath->AIRealRectSet(&bleedRect, diff.left, diff.top, diff.right, diff.bottom);
        
        if (TickMarkSettings().TickMarkStyle() == SafeguardFile::TickMarkStyle::Inner)
        {
            sAIRealMath->AIRealRectSet(&bleedRect, 0, 0, 0, 0);
        }
        
    }
    else if (pt == SafeguardFile::ProductType::BusinessStat)
    {
        if (diff.left < 21) diff.left = 21;
        if (diff.top < 21) diff.top = 21;
        if (diff.right < 21) diff.right = 21;
        if (diff.bottom < 21) diff.bottom = 21;
        sAIRealMath->AIRealRectSet(&bleedRect, diff.left, diff.top, diff.right, diff.bottom);
    }
    else if (pt == SafeguardFile::ProductType::Continuous)
    {
        if (diff.left < 9) diff.left = 9;
        if (diff.top < 9) diff.top = 9;
        if (diff.right < 9) diff.right = 9;
        if (diff.bottom < 9) diff.bottom = 9;
        sAIRealMath->AIRealRectSet(&bleedRect, diff.left, diff.top, diff.right, diff.bottom);
    }
    else //No bleed
    {
        sAIRealMath->AIRealRectSet(&bleedRect, 0, 0, 0, 0);
    }
    
    return bleedRect;
}

void BleedInfo::FillBleedInfoFromPlateDTO(const PlateBleedInfo::PlateDTO* dto, bool changeArtboardName)
{
    ShouldDrawBleedInfo(dto->shouldDrawBleedInfo);
    ShouldPrint(dto->shouldPrint);
    if (changeArtboardName)
    {
        ArtboardName(dto->artboardName);
    }
    //.ShouldAddCmykBlocks(dto->shouldAddCmykBlocks) //Do not set cmyk blocks here or we'll overwrite what's been set by the file type and color list
    TickMarkStyle(SafeguardFile::TickMarkStyle(dto->tmStyle));
    for ( auto color : dto->c )
    {
        ColorList().SetColorMethod(color.colorName, SafeguardFile::InkMethod(color.method) );
    }
}

void BleedInfo::StoreInPluginArt() const
{
    BleedInfoWriter(this, make_shared<BleedInfoToArtStrategy>(bleedInfoPluginArt));
}

void BleedInfo::ReadFromPluginArt()
{
    bool sp = shouldPrint;
    
    if (bleedInfoPluginArt)
    {
        DictionaryWriter dw(bleedInfoPluginArt);
        string json;
        dw.GetStringDataFromIdentifier(json, PLATE_BLEEDINFO);
        
        PlateBleedInfo::PlateDTO dto(json);
        
        dto.shouldPrint = sp;
        
        FillBleedInfoFromPlateDTO(&dto, false);
    }
}

AIArtHandle BleedInfo::Draw(AIArtHandle existingArt)
{
    BleedInfoDrawableController controller(*this); //Sets up all drawers
    bleedInfoPluginArt = controller.Draw();
        
    //These null-check bleedInfoPluginArt
    DictionaryWriter dw;
    dw.AddAIArtHandleToArrayInDictionary(bleedInfoPluginArt, SG_BLEEDINFO_ARTHANDLES);
            
    return bleedInfoPluginArt;
}

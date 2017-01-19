//
//  FileNameDateDrawer.h
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#ifndef __SafeguardTools__FileNameDateDrawer__
#define __SafeguardTools__FileNameDateDrawer__

#include "BleedTextInfoDrawer.h"
#include "PlateNumber.h"
#include <ctime>
#include "IDrawer.h"
#include "IDrawable.hpp"

struct FileNameDateDrawerSettings
{
    FileNameDateDrawerSettings(SafeguardFile::ProductType pt, AIRealRect artboardBounds, SafeguardFile::PlateNumber plateNumber, string token, tm lastModified) :
    pt(pt),
    artboardBounds(artboardBounds),
    plateNumber(plateNumber),
    token(token),
    lastModified(lastModified) {};
    
    SafeguardFile::ProductType pt;
    AIRealRect artboardBounds;
    SafeguardFile::PlateNumber plateNumber;
    string token;
    tm lastModified;
};

constexpr auto FILENAMEDATE_ARTHANDLE =            "__bt_filenamedate_arthandle__";

namespace SafeguardFile
{
    class BleedInfo;
    
    class FileNameDateDrawer : public BleedTextInfoDrawer
    {
    public:
        string GetDictionaryLabel(AIArtHandle resultArt) const override { return FILENAMEDATE_ARTHANDLE + DictionaryWriter::GetUIDStringForArt(resultArt); };
    protected:
        FileNameDateDrawer(AIRealRect artboardBounds, AIRealPoint anchor, PlateNumber plateNumber, string token, tm lastModified);
        void PutPlateNumberDateStringInTextRange(ATE::ITextRange& targetRange) const;
        
        PlateNumber plateNumber;
        string token;
        tm lastModified;
    };
    
    class LaserFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        LaserFileNameDateDrawer(AIRealRect artboardBounds, PlateNumber plateNumber, string token, tm lastModified);
    private:
        AIArtHandle Draw(AIArtHandle resultGroup) const override;
    };
    
    class ContinuousFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        ContinuousFileNameDateDrawer(AIRealRect artboardBounds, PlateNumber plateNumber, string token, tm lastModified);
    private:
        AIArtHandle Draw(AIArtHandle resultGroup) const override;
    };
    
    class BusStatFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        BusStatFileNameDateDrawer(AIRealRect artboardBounds, PlateNumber plateNumber, string token, tm lastModified);
    private:
        AIArtHandle Draw(AIArtHandle resultGroup) const override;
    };
    
    class NoneFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        NoneFileNameDateDrawer() : FileNameDateDrawer({}, {}, PlateNumber(), "", {}) {};
    private:
        AIArtHandle Draw(AIArtHandle resultGroup) const override { return NULL; };
    };
    
    class FileNameDateDrawable : public IDrawable
    {
    public:
        FileNameDateDrawable(FileNameDateDrawerSettings settings)
        {
            drawer = DrawerFactory().GetDrawer(settings);
        }
    };
}

using SafeguardFile::FileNameDateDrawable;
template <>
class DrawableFactoryImpl<FileNameDateDrawerSettings>
{
public:
    static std::shared_ptr<IDrawable> GetDrawable(FileNameDateDrawerSettings settings, AIArtHandle pluginArt)
    {
        if (true)
        {
            return make_shared<FileNameDateDrawable>(settings);
        }
        return nullptr;
    };
};

template <>
class DrawerFactoryImpl<FileNameDateDrawerSettings>
{
public:
    static shared_ptr<IDrawer> GetDrawer(FileNameDateDrawerSettings settings)
    {
        if (settings.pt == SafeguardFile::ProductType::BusinessStat)
        {
            return make_shared<SafeguardFile::BusStatFileNameDateDrawer>(settings.artboardBounds, settings.plateNumber, settings.token, settings.lastModified);
        }
        else if (settings.pt == SafeguardFile::ProductType::Continuous)
        {
            return make_shared<SafeguardFile::ContinuousFileNameDateDrawer>(settings.artboardBounds, settings.plateNumber, settings.token, settings.lastModified);
        }
        else if (settings.pt == SafeguardFile::ProductType::CutSheet || settings.pt == SafeguardFile::ProductType::Envelope)
        {
            return make_shared<SafeguardFile::LaserFileNameDateDrawer>(settings.artboardBounds, settings.plateNumber, settings.token, settings.lastModified);
        }
        
        return make_shared<SafeguardFile::NoneFileNameDateDrawer>();
    };
};


#endif /* defined(__SafeguardTools__FileNameDateDrawer__) */

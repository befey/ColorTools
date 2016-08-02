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

namespace SafeguardFile
{
    class BleedInfo;
    
    class FileNameDateDrawer : public BleedTextInfoDrawer
    {
    protected:
        FileNameDateDrawer(AIRealRect bounds, AIRealPoint anchor, PlateNumber plateNumber, string token, tm lastModified);
        
        PlateNumber plateNumber;
        string token;
        tm lastModified;
        
        void PutPlateNumberDateStringInTextRange(ATE::ITextRange& targetRange) const;
    };
    
    class LaserFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        LaserFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified);
    private:
        AIArtHandle DoDraw() const override;
    };
    
    class ContinuousFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        ContinuousFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified);
    private:
        AIArtHandle DoDraw() const override;
    };
    
    class BusStatFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        BusStatFileNameDateDrawer(AIRealRect bounds, PlateNumber plateNumber, string token, tm lastModified);
    private:
        AIArtHandle DoDraw() const override;
    };
}

#endif /* defined(__SafeguardTools__FileNameDateDrawer__) */

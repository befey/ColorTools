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

namespace SafeguardFile
{
    class BleedInfo;
    
    class FileNameDateDrawer : public BleedTextInfoDrawer
    {
    protected:
        FileNameDateDrawer(shared_ptr<BleedInfo> info);
    };
    
    class LaserFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        LaserFileNameDateDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
    
    class ContinuousFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        ContinuousFileNameDateDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
    
    class BusStatFileNameDateDrawer : public FileNameDateDrawer
    {
    public:
        BusStatFileNameDateDrawer(shared_ptr<BleedInfo> info);
        AIArtHandle Draw();
    };
}

#endif /* defined(__SafeguardTools__FileNameDateDrawer__) */

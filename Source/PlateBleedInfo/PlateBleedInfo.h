//
//  PlateBleedInfo.h
//  SafeguardTools
//
//  Created by T431962 on 6/7/16.
//
//

#ifndef __SafeguardTools__PlateBleedInfo__
#define __SafeguardTools__PlateBleedInfo__

#include "AIArtboard.h"
#include "AIATETextUtil.h"
#include "AIATEPaint.h"

extern AIArtSuite* sAIArt;
extern AIPluginGroupSuite* sAIPluginGroup;
extern AIPathSuite* sAIPath;
extern AIArtboardSuite* sAIArtboard;
extern AITextFrameSuite* sAITextFrame;
extern AIATEPaintSuite* sAIATEPaint;
extern AIUndoSuite* sAIUndo;

namespace PlateBleedInfo
{
    constexpr auto PLATE_BLEED_INFO_GROUP_LABEL =             "__plate_bleed_info__";
    
    void AddPlateBleedInfo();
    void RemovePlateBleedInfo();
    void UpdatePlateBleedInfo(AINotifierMessage* message);
    ASErr PluginGroupNotify(AIPluginGroupMessage* message);
    ASErr PluginGroupUpdate(AIPluginGroupMessage* message);
}

#endif /* defined(__SafeguardTools__PlateBleedInfo__) */

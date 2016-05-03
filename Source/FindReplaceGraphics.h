//
//  FindReplaceGraphics.h
//  SafeguardTools
//
//  Created by T431962 on 5/21/15.
//
//

#ifndef __SafeguardTools__FindReplaceGraphics__
#define __SafeguardTools__FindReplaceGraphics__

#include "ReplaceData.h"

int FindAndReplace(std::unique_ptr<ReplaceData> &data);
void fillArtSet( AIArtSet &artSet, ColorToolsUIController::ChangeIn changeIn);

void adjustColor(AIColor *color, void* userData, AIErr *result, AIBoolean *altered);

#endif /* defined(__SafeguardTools__FindReplaceGraphics__) */

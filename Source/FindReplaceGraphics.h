//
//  FindReplaceGraphics.h
//  ColorTools
//
//  Created by T431962 on 5/21/15.
//
//

#ifndef __ColorTools__FindReplaceGraphics__
#define __ColorTools__FindReplaceGraphics__

class ReplaceData;

int FindAndReplace(ReplaceData* data);
void fillArtSet( AIArtSet &artSet, int changeIn);

void adjustColor(AIColor *color, void* userData, AIErr *result, AIBoolean *altered);

#endif /* defined(__ColorTools__FindReplaceGraphics__) */

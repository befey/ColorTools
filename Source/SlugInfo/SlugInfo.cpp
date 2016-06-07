//
//  SlugInfo.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/7/16.
//
//

#include "SlugInfo.h"
#include "SafeguardToolsPlugin.h"


void SlugInfo::UpdateSlugInfo()
{
    AIArtHandle pluginGroupArt, editArt, boundingBoxArt;
    sAIArt->NewArt(kPluginArt, kPlaceAboveAll, NULL, &pluginGroupArt);
    sAIPluginGroup->UseAIPluginGroup(pluginGroupArt, gPlugin->GetCreateSlugInfoPluginGroupHandle());
    
    sAIArt->NewArt(kGroupArt, kPlaceAboveAll, NULL, &editArt);
    sAIArt->NewArt(kPathArt, kPlaceAboveAll, NULL, &boundingBoxArt);
    
    AIPathSegment segments[4];
    segments[0] = { .corner = true, .in = {.h = 0, .v = 0}, .out = {.h = 0, .v = 0}, .p = {.h = 0, .v = 0} };
    segments[1] = { .corner = true, .in = {.h = 20, .v = 0}, .out = {.h = 20, .v = 0}, .p = {.h = 20, .v = 0} };
    segments[2] = { .corner = true, .in = {.h = 20, .v = 20}, .out = {.h = 20, .v = 20}, .p = {.h = 20, .v = 20} };
    segments[3] = { .corner = true, .in = {.h = 0, .v = 20}, .out = {.h = 0, .v = 20}, .p = {.h = 0, .v = 20} };

    sAIPath->SetPathSegments(boundingBoxArt, 0, 4, segments);
    sAIPath->SetPathClosed(boundingBoxArt, true);
    
    sAIArt->ReorderArt(boundingBoxArt, kPlaceInsideOnTop, editArt);
    
    sAIPluginGroup->SetPluginArtEditArt(pluginGroupArt, editArt);
}
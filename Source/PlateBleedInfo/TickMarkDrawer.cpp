//
//  TickMarkDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "TickMarkDrawer.h"
#include "BleedInfo.h"

using SafeguardFile::TickMarkDrawer;
using SafeguardFile::OuterTickMarkDrawer;
using SafeguardFile::InnerTickMarkDrawer;
using SafeguardFile::ContinuousTickMarkDrawer;
using SafeguardFile::BleedInfo;

TickMarkDrawer::TickMarkDrawer(shared_ptr<BleedInfo> info) : p_BleedInfo(info) {};
OuterTickMarkDrawer::OuterTickMarkDrawer(shared_ptr<BleedInfo> info) : TickMarkDrawer(info) {};
InnerTickMarkDrawer::InnerTickMarkDrawer(shared_ptr<BleedInfo> info) : TickMarkDrawer(info) {};
ContinuousTickMarkDrawer::ContinuousTickMarkDrawer(shared_ptr<BleedInfo> info) : TickMarkDrawer(info) {};

AIArtHandle OuterTickMarkDrawer::Draw()
{

//        sAIArt->NewArt(kGroupArt, kPlaceAboveAll, NULL, &editArt);
//        sAIArt->NewArt(kPathArt, kPlaceAboveAll, NULL, &boundingBoxArt);
//
//        AIPathSegment segments[4];
//        segments[0] = { .corner = true, .p = {.h = rect.left, .v = rect.top}, .in = {.h = rect.left, .v = rect.top}, .out = {.h = rect.left, .v = rect.top}};
//        segments[1] = { .corner = true, .p = {.h = rect.right, .v = rect.top}, .in = {.h = rect.right, .v = rect.top}, .out = {.h = rect.right, .v = rect.top} };
//        segments[2] = { .corner = true, .p = {.h = rect.right, .v = rect.bottom}, .in = {.h = rect.right, .v = rect.bottom}, .out = {.h = rect.right, .v = rect.bottom} };
//        segments[3] = { .corner = true, .p = {.h = rect.left, .v = rect.bottom}, .in = {.h = rect.left, .v = rect.bottom}, .out = {.h = rect.left, .v = rect.bottom} };
//
//        sAIPath->SetPathSegments(boundingBoxArt, 0, 4, segments);
//        sAIPath->SetPathClosed(boundingBoxArt, true);
//
//        sAIArt->ReorderArt(boundingBoxArt, kPlaceInsideOnTop, editArt);
//
//        sAIPluginGroup->SetPluginArtEditArt(pluginGroupArt, editArt);
    return NULL;
}
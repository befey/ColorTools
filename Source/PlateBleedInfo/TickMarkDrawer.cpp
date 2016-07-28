//
//  TickMarkDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 6/10/16.
//
//

#include "TickMarkDrawer.h"

using SafeguardFile::TickMarkDrawer;

TickMarkDrawer::TickMarkDrawer(TickMarkSettings settings) : settings(settings) {};

AIArtHandle TickMarkDrawer::Draw()
{
    AIArtHandle tickMarkGroupArt = NULL;
    if (settings.DrawInner())
    {
        tickMarkGroupArt = DrawInner(tickMarkGroupArt);
    }
    if (settings.DrawOuter())
    {
        tickMarkGroupArt = DrawOuter(tickMarkGroupArt);
    }
    
    return tickMarkGroupArt;
}

AIArtHandle TickMarkDrawer::DrawOuter(AIArtHandle tickMarkGroupArt)
{
    if (tickMarkGroupArt == NULL)
    {
        sAIArt->NewArt(kGroupArt, kPlaceAboveAll, NULL, &tickMarkGroupArt);
    }
    
    AIArtHandle tickMarkArt;
    sAIArt->NewArt(kPathArt, kPlaceInsideOnTop, tickMarkGroupArt, &tickMarkArt);

    AIPathSegment segments[3];
    segments[0] = { .corner = true, .p = {.h = settings.Bounds().left, .v = settings.Bounds().top - settings.Offset()}, .in = {.h = settings.Bounds().left, .v = settings.Bounds().top - settings.Offset()}, .out = {.h = settings.Bounds().left, .v = settings.Bounds().top - settings.Offset()} };
    segments[1] = { .corner = true, .p = {.h = settings.Bounds().left, .v = settings.Bounds().top}, .in = {.h = settings.Bounds().left, .v = settings.Bounds().top}, .out = {.h = settings.Bounds().left, .v = settings.Bounds().top} };
    segments[2] = { .corner = true, .p = {.h = settings.Bounds().left - settings.Offset(), .v = settings.Bounds().top}, .in = {.h = settings.Bounds().left - settings.Offset(), .v = settings.Bounds().top}, .out = {.h = settings.Bounds().left - settings.Offset(), .v = settings.Bounds().top} };
    
    sAIPath->SetPathSegments(tickMarkArt, 0, 3, segments);
    sAIPath->SetPathClosed(tickMarkArt, false);
    
    AIPathStyle currPathStyle;
    sAIPathStyle->GetPathStyle(tickMarkArt, &currPathStyle);
    currPathStyle.stroke.color = settings.Color();
    currPathStyle.strokePaint = true;
    currPathStyle.stroke.width = settings.Weight();
    currPathStyle.fillPaint = false;
    
    return tickMarkGroupArt;
}
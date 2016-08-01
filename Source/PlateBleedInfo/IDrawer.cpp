//
//  IDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/29/16.
//
//

#include "IDrawer.h"

using SafeguardFile::IDrawer;

AIArtHandle IDrawer::Draw() const
{
    return DoDraw();
}
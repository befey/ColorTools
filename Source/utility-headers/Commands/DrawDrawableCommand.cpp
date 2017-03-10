//
//  DrawDrawableCommand.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/9/17.
//
//

#include "DrawDrawableCommand.hpp"


DrawDrawableCommand::DrawDrawableCommand(shared_ptr<IDrawable> drawable, BtArtHandle prep)
:
drawable(drawable),
prep(prep)
{}

bool DrawDrawableCommand::Execute() const
{
    if (drawable != nullptr && !prep.Null())
    {
        drawable->Clear(prep);
        drawable->Draw(prep);
    }
    
    return true;
}

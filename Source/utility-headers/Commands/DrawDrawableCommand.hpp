//
//  DrawDrawableCommand.hpp
//  SafeguardTools
//
//  Created by T431962 on 3/9/17.
//
//

#ifndef DrawDrawableCommand_hpp
#define DrawDrawableCommand_hpp

#include "IBtCommand.hpp"
#include "IDrawable.hpp"
#include "BtArtHandle.hpp"

class DrawDrawableCommand : public IBtCommand
{
public:
    DrawDrawableCommand(shared_ptr<IDrawable> drawable, BtArtHandle prep);
    
    bool Execute() const override;
private:
    
    shared_ptr<IDrawable> drawable;
    BtArtHandle prep;
};

#endif /* DrawDrawableCommand_hpp */

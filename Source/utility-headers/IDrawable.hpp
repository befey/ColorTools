//
//  IDrawable.hpp
//  SafeguardTools
//
//  Created by T431962 on 12/20/16.
//
//

#ifndef IDrawable_h
#define IDrawable_h

#include "IDrawer.h"

class IDrawable
{
public:
    virtual AIArtHandle Draw(AIArtHandle resultArt)
    {
        return drawer->Draw(resultArt);
    };
protected:
    IDrawable() {};
    ~IDrawable() {};
    
    shared_ptr<IDrawer> drawer;
};

#endif /* IDrawable_h */

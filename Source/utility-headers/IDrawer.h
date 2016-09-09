//
//  IDrawer.h
//  SafeguardTools
//
//  Created by T431962 on 7/29/16.
//
//

#ifndef IDrawer_h
#define IDrawer_h

class IDrawer
{
public:
    AIArtHandle Draw() const { return DoDraw(); };
protected:
    ~IDrawer() {};
private:
    virtual AIArtHandle DoDraw() const = 0;
};

#endif /* IDrawer_h */
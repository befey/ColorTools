//
//  IDrawer.h
//  SafeguardTools
//
//  Created by T431962 on 7/29/16.
//
//

#ifndef IDrawer_h
#define IDrawer_h

namespace SafeguardFile
{
    class IDrawer
    {
    public:
        AIArtHandle Draw() const;
    protected:
        ~IDrawer() {};
    private:       
        virtual AIArtHandle DoDraw() const = 0;
    };
}

#endif /* IDrawer_h */

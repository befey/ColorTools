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
    virtual AIArtHandle Draw(AIArtHandle resultArt) const = 0;
    virtual string GetDictionaryLabel() const = 0;
protected:
    IDrawer(AIRealRect bounds) : bounds(bounds) { };
    AIRealRect bounds;
};

template <class T>
class DrawerFactoryImpl;

class DrawerFactory
{
public:
    template <class T>
    std::shared_ptr<IDrawer> GetDrawer(T drawableSettings)
    {
        return DrawerFactoryImpl<T>::GetDrawer(drawableSettings);
    }
};

#endif /* IDrawer_h */

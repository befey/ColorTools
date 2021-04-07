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
#include "DictionaryWriter.h"

template <class T>
class DrawableFactoryImpl;

class IDrawable
{
public:
    virtual AIArtHandle Draw(AIArtHandle resultArt)
    {
        AIArtHandle drawableResult = drawer->Draw(resultArt);
        
        if (drawableResult)
        {
            DictionaryWriter(resultArt).AddArtHandleToDictionary(drawableResult, drawer->GetDictionaryLabel(resultArt));
        }
        
        return drawableResult;
    };
    
    virtual void Clear(AIArtHandle resultArt)
    {
        AIArtHandle previous = DictionaryWriter(resultArt).GetArtHandleFromIdentifier(drawer->GetDictionaryLabel(resultArt));
        
        if (previous)
        {
            sAIArt->DisposeArt(previous);
        }
    };
    
    const shared_ptr<IDrawer> GetDrawer() const { return drawer; };
protected:
    IDrawable() {};
    ~IDrawable() {};
    
    shared_ptr<IDrawer> drawer;
};

class DrawableFactory
{
public:
    template <class T>
    std::shared_ptr<IDrawable> GetDrawable(T drawableSettings, AIArtHandle pluginArt)
    {
        return DrawableFactoryImpl<T>::GetDrawable(drawableSettings, pluginArt);
    }
};

#endif /* IDrawable_h */

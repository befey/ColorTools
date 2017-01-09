//
//  SgSymbolDrawer.hpp
//  SafeguardTools
//
//  Created by T431962 on 11/1/16.
//
//

#ifndef SgSymbolDrawer_hpp
#define SgSymbolDrawer_hpp

#include "BtTransformArt.hpp"
#include "AIArt.h"
#include "AISymbol.h"
#include "AIHardSoft.h"
#include "AIRealMath.h"
#include "AIArtSet.h"
#include "AIDocumentList.h"
#include "AIDocument.h"
#include "AIPathStyle.h"
#include "IDrawer.h"
#include "IDrawable.hpp"

extern AIArtSuite* sAIArt;
extern AISymbolSuite* sAISymbol;
extern AIHardSoftSuite* sAIHardSoft;
extern AIRealMathSuite* sAIRealMath;
extern AIArtSetSuite* sAIArtSet;
extern AIDocumentListSuite* sAIDocumentList;
extern AIDocumentSuite* sAIDocument;
extern AIPathStyleSuite* sAIPathStyle;

struct SgSymbolDrawerSettings
{
    SgSymbolDrawerSettings(AIRealRect artboardBounds, AIRealRect symbolBounds, string symbolName) :
    artboardBounds(artboardBounds),
    symbolBounds(symbolBounds),
    symbolName(symbolName) {};
    
    AIRealRect artboardBounds;
    AIRealRect symbolBounds;
    string symbolName;
};

constexpr auto SGSYMBOL_ARTHANDLE =            "__bt_sgsymbol_arthandle__";

namespace SafeguardFile
{
    constexpr auto SG_SYMBOL_FILENAME = "sg_Symbols.ai";
    
    class SgSymbolDrawer : public IDrawer
    {
    public:
        SgSymbolDrawer(AIRealRect bounds, AIRealRect symbolBounds, string symbolName) : IDrawer(bounds), symbolBounds(symbolBounds), symbolName(symbolName) {};
        string GetDictionaryLabel() const override { return SGSYMBOL_ARTHANDLE + symbolName; };
        
    private:
        const AIRealRect symbolBounds;
        const string symbolName;
        
        AIArtHandle Draw(AIArtHandle resultArt) const override;
        
        AIPatternHandle LoadSymbolFromFile() const;
    };
    
    class SgSymbolDrawable : public IDrawable
    {
    public:
        SgSymbolDrawable(SgSymbolDrawerSettings settings)
        {
            drawer = DrawerFactory().GetDrawer(settings);
        }
    };
}

using SafeguardFile::SgSymbolDrawable;
template <>
class DrawableFactoryImpl<SgSymbolDrawerSettings>
{
public:
    static std::shared_ptr<IDrawable> GetDrawable(SgSymbolDrawerSettings settings, AIArtHandle pluginArt)
    {
        if (true)
        {
            return make_shared<SgSymbolDrawable>(settings);
        }
        return nullptr;
    };
};

using SafeguardFile::SgSymbolDrawer;
template <>
class DrawerFactoryImpl<SgSymbolDrawerSettings>
{
public:
    static shared_ptr<IDrawer> GetDrawer(SgSymbolDrawerSettings settings)
    {
        return make_shared<SgSymbolDrawer>(settings.artboardBounds, settings.symbolBounds, settings.symbolName);
    };
};


#endif /* SgSymbolDrawer_hpp */

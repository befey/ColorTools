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
    SgSymbolDrawerSettings(AIRealRect artboardBounds, string symbolName, bool shouldDraw, bool shouldDrawBleedInfo) :
    artboardBounds(artboardBounds),
    symbolName(symbolName),
    shouldDraw(shouldDraw),
    shouldDrawBleedInfo(shouldDrawBleedInfo) {};
    
    AIRealRect artboardBounds;
    string symbolName;
    bool shouldDraw;
    bool shouldDrawBleedInfo;
};

constexpr auto SGSYMBOL_ARTHANDLE =            "__bt_sgsymbol_arthandle__";

namespace SafeguardFile
{
    constexpr auto SG_SYMBOL_FILENAME = "sg_Symbols.ai";
    
    class SgSymbolDrawer : public IDrawer
    {
    public:
        SgSymbolDrawer(AIRealRect artboardBounds, AIRealRect symbolBounds, string symbolName) : IDrawer(artboardBounds), symbolBounds(symbolBounds), symbolName(symbolName) {};
        string GetDictionaryLabel(AIArtHandle resultArt) const override { return SGSYMBOL_ARTHANDLE + symbolName + DictionaryWriter::GetUIDStringForArt(resultArt); };
        
    private:
        const AIRealRect symbolBounds;
        const string symbolName;
        
        AIArtHandle Draw(AIArtHandle resultArt) const override;
        
        AIPatternHandle LoadSymbolFromFile() const;
    };
    
    class NoneSgSymbolDrawer : public SgSymbolDrawer
    {
    public:
        NoneSgSymbolDrawer(string symbolName) : SgSymbolDrawer({}, {}, symbolName) {};
        AIArtHandle Draw(AIArtHandle resultGroup) const override { return nullptr; };
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
        return make_shared<SgSymbolDrawable>(settings);
    };
};

using SafeguardFile::SgSymbolDrawer;
using SafeguardFile::NoneSgSymbolDrawer;
template <>
class DrawerFactoryImpl<SgSymbolDrawerSettings>
{
public:
    static shared_ptr<IDrawer> GetDrawer(SgSymbolDrawerSettings settings)
    {
        if (!settings.shouldDrawBleedInfo)
        {
            return make_shared<NoneSgSymbolDrawer>(settings.symbolName);
        }
        
        if (settings.shouldDraw)
        {
            AIRealRect abBounds = settings.artboardBounds;
            AIRealRect symbolBounds;
            if (settings.symbolName == SafeguardFile::AI_CMYK_BLOCKS)
            {
                symbolBounds = { //CMYK Blocks are 325x25px
                    .left = abBounds.left + ((abBounds.right - abBounds.left)/2) - (325/2),
                    .top = abBounds.top + 5 + 25,
                    .right = abBounds.right - ((abBounds.right - abBounds.left)/2) + (325/2),
                    .bottom = abBounds.top + 5
                };
                return make_shared<SgSymbolDrawer>(settings.artboardBounds, symbolBounds, settings.symbolName);
            }
            if (settings.symbolName == SafeguardFile::AI_CONTINUOUS_REG_TARGET)
            {
                symbolBounds = { //Reg block is 24.3x36px
                    .left = abBounds.left,
                    .top = abBounds.top - 42,
                    .right = abBounds.left + 24.3,
                    .bottom = abBounds.top - 42 - 36
                };
                return make_shared<SgSymbolDrawer>(settings.artboardBounds, symbolBounds, settings.symbolName);
            }
        }
        
        return make_shared<NoneSgSymbolDrawer>(settings.symbolName);
    };
};


#endif /* SgSymbolDrawer_hpp */

//
//  SgSymbolDrawer.hpp
//  SafeguardTools
//
//  Created by T431962 on 11/1/16.
//
//

#ifndef SgSymbolDrawer_hpp
#define SgSymbolDrawer_hpp

#include "IDrawer.h"
#include "BtTransformArt.hpp"
#include "AIArt.h"
#include "AISymbol.h"
#include "AIHardSoft.h"
#include "AIRealMath.h"
#include "AIArtSet.h"
#include "AIDocumentList.h"
#include "AIDocument.h"
#include "AIPathStyle.h"

extern AIArtSuite* sAIArt;
extern AISymbolSuite* sAISymbol;
extern AIHardSoftSuite* sAIHardSoft;
extern AIRealMathSuite* sAIRealMath;
extern AIArtSetSuite* sAIArtSet;
extern AIDocumentListSuite* sAIDocumentList;
extern AIDocumentSuite* sAIDocument;
extern AIPathStyleSuite* sAIPathStyle;

namespace SafeguardFile
{
    constexpr auto SG_SYMBOL_FILENAME = "sg_Symbols.ai";
    
    class SgSymbolDrawer : public IDrawer
    {
    public:
        SgSymbolDrawer(AIRealRect bounds, string symbolName) : bounds(bounds), symbolName(symbolName) {};
        
    private:
        const AIRealRect bounds;
        const string symbolName;
        
        AIArtHandle DoDraw(AIArtHandle resultGroup) const override;
        
        AIPatternHandle LoadSymbolFromFile() const;
    };
}


#endif /* SgSymbolDrawer_hpp */

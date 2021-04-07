//
//  PlacedArtFileDrawer.cpp
//  SafeguardTools
//
//  Created by T431962 on 11/1/16.
//
//

#include "SgSymbolDrawer.hpp"
#include "SafeguardFileConstants.h"
#include "GetIllustratorErrorCode.h"
#include "ArtTree.h"
#include "BtTransformArt.hpp"

using SafeguardFile::SgSymbolDrawer;

AIArtHandle SgSymbolDrawer::Draw(AIArtHandle resultGroup) const
{
    AIArtHandle placedSymbol = nullptr;
    AIPatternHandle symbol = LoadSymbolFromFile();
    
    if (symbol != nullptr)
    {
        sAISymbol->NewInstanceAtLocation(symbol, GetCenterOfRect(symbolBounds), kPlaceInsideOnBottom, resultGroup, &placedSymbol);
    }
    
    return placedSymbol;
}

AIPatternHandle SgSymbolDrawer::LoadSymbolFromFile() const
{
    AIPatternHandle symbol = nullptr;
    sAISymbol->GetSymbolPatternByName(ai::UnicodeString(symbolName), &symbol);
    
    if (symbol == nullptr)
    {
        ai::FilePath pathToFile;
        sAIFolders->FindFolder(kAIPresetSymbolsFolderType, false, pathToFile);
        pathToFile.AddComponent(ai::FilePath(ai::UnicodeString(SG_SYMBOL_FILENAME)));
        
        AIDocumentHandle symbolDoc = nullptr;
        sAIPathStyle->ImportStyles(pathToFile, &symbolDoc);
        
        AIPatternHandle curr = nullptr;
        AIErr err = sAISymbol->GetSymbolByNameFromDocument(ai::UnicodeString(symbolName), &curr, symbolDoc);
        string error = GetIllustratorErrorCode(err);
        
        sAISymbol->RetargetForCurrentDocument(curr, &symbol);
    }
    
    return symbol;
}

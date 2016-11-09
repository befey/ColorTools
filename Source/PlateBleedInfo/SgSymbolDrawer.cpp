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

AIArtHandle SgSymbolDrawer::DoDraw(AIArtHandle resultGroup) const
{
    AIArtHandle placedSymbol = NULL;
    AIPatternHandle symbol = LoadSymbolFromFile();
    
    if (symbol != NULL)
    {
        AIErr err = sAISymbol->NewInstanceAtLocation(symbol, GetCenterOfRect(bounds), kPlaceInsideOnBottom, resultGroup, &placedSymbol);
        string error = GetIllustratorErrorCode(err);
        int foo = 0;
    }
    
    return placedSymbol;
}

AIPatternHandle SgSymbolDrawer::LoadSymbolFromFile() const
{
    AIPatternHandle symbol = NULL;
    sAISymbol->GetSymbolPatternByName(ai::UnicodeString(symbolName), &symbol);
    
    if (symbol == NULL)
    {
        ai::FilePath pathToFile;
        sAIFolders->FindFolder(kAIPresetSymbolsFolderType, false, pathToFile);
        pathToFile.AddComponent(ai::FilePath(ai::UnicodeString("sg_Symbols.ai")));
        
        AIDocumentHandle symbolDoc = NULL;
        sAIPathStyle->ImportStyles(pathToFile, &symbolDoc);
        
        ai::int32 count = 0;
        sAISymbol->CountSymbolPatternsFromDocument(&count, symbolDoc);
        
        AIPatternHandle curr = NULL;
        AIErr err = sAISymbol->GetSymbolByNameFromDocument(ai::UnicodeString(symbolName), &curr, symbolDoc);
        string error = GetIllustratorErrorCode(err);
        
        sAISymbol->RetargetForCurrentDocument(curr, &symbol);
    }
    
    return symbol;
}

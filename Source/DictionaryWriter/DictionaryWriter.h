//
//  DictionaryWriter.h
//  SafeguardTools
//
//  Created by T431962 on 7/12/16.
//
//

#ifndef DictionaryWriter_hpp
#define DictionaryWriter_hpp

//=================================
// included dependencies
#include "AIDictionary.h"
#include "AIDocument.h"
#include "AIArt.h"
#include "AIUID.h"
#include <string>

//=================================
// forward declared dependencies
extern AIDictionarySuite *sAIDictionary;
extern AIDocumentSuite *sAIDocument;
extern AIArtSuite *sAIArt;
extern AIUIDSuite *sAIUID;
extern AIUIDREFSuite *sAIUIDREF;
extern AIUIDUtilsSuite *sAIUIDUtils;
extern AIEntrySuite *sAIEntry;

class DictionaryWriter
{
public:
    DictionaryWriter();
    ~DictionaryWriter();
    
    AIBoolean RemoveIdentifierFromDictionary(string identifier, int CAIndex = -1);
    
    AIReal GetAIRealFromIdentifier(string identifier, int CAIndex = -1);
    AIBoolean AddAIRealToDictionary(AIReal value, string identifier, int CAIndex = -1);
    
    //********* AIArtHandle Functions *****************************************************
    AIArtHandle GetArtHandleFromIdentifier(string identifier, int CAIndex = -1);
    AIBoolean CheckDictionaryForArtObjectWithIdentifier(string identifier, int CAIndex = -1);
    AIBoolean AddArtHandleToDictionary(AIArtHandle currArt, string identifier, int CAIndex = -1);
    
private:
    AIDictionaryRef dictionary;
    
    AIDictKey GenerateKey(string identifier, int CAIndex = -1);
    //identifier -- the word part of the key... usually the variable name we're storing
    //CAIndex -- the crop area index
    //Returns the existing key, or a new key reference if none was found
    
    string GenerateUIDString(string identifier, int CAIndex = -1);
    //Returns a string in the format "identifier_CAIndex"
    
    AIUIDRef GetUIDRefFromIdentifier(string identifier, int CAIndex = -1);
    
    AIArtHandle GetArtHandleFromUIDRef(AIUIDRef uidRef);
    //Returns the handle to the art object with the associated id, or a NULL handle if it couldn't be found
    ASBoolean CheckArtHandleFromUIDRef(AIUIDRef uidRef);
    //Returns TRUE if the uid points to an art handle in the document, FALSE if its NULL or invalid
};

#endif /* DictionaryWriter_h */

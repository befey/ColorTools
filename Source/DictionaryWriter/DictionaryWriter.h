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
#include "AIArray.h"
#include <string>
#include <vector>
#include "BtColor.h"

//=================================
// forward declared dependencies
extern AIDictionarySuite *sAIDictionary;
extern AIDocumentSuite *sAIDocument;
extern AIArtSuite *sAIArt;
extern AIUIDSuite *sAIUID;
extern AIUIDREFSuite *sAIUIDREF;
extern AIUIDUtilsSuite *sAIUIDUtils;
extern AIEntrySuite *sAIEntry;
extern AIArraySuite *sAIArray;

class DictionaryWriter
{
public:
    DictionaryWriter();
    DictionaryWriter(AIDictionaryRef dictionary) : dictionary(dictionary) {};
    DictionaryWriter(AIArtHandle art);
    ~DictionaryWriter();
    
    AIBoolean RemoveIdentifierFromDictionary(string identifier, int CAIndex = -1);
    AIBoolean CheckDictionaryForIdentifier(string identifier, int CAIndex = -1);

    AIReal GetAIRealFromIdentifier(string identifier, int CAIndex = -1);
    AIBoolean AddAIRealToDictionary(AIReal value, string identifier, int CAIndex = -1);
    
    //********* AIArtHandle Functions *****************************************************
    AIArtHandle GetArtHandleFromIdentifier(string identifier, int CAIndex = -1);
    AIBoolean CheckDictionaryForArtObjectWithIdentifier(string identifier, int CAIndex = -1);
    AIBoolean AddArtHandleToDictionary(AIArtHandle currArt, string identifier, int CAIndex = -1);
    
    AIBoolean GetStringDataFromIdentifier(string& data, string identifier, int CAIndex = -1);
    AIBoolean AddStringDataToDictionary(string data, string identifier, int CAIndex = -1);
    
    AIBoolean GetArrayDataFromIdentifier(AIArrayRef& data, string identifier, int CAIndex = -1);
    AIBoolean AddArrayDataToDictionary(AIArrayRef data, string identifier, int CAIndex = -1);
    
    AIBoolean GetVectorOfAIArtHandleFromIdentifier(vector<AIArtHandle>& handles, string identifier, int CAIndex = -1);
    AIBoolean AddVectorOfAIArtHandleToDictionary(vector<AIArtHandle> handles, string identifier, int CAIndex = -1);
    AIBoolean AddAIArtHandleToArrayInDictionary(AIArtHandle art, string identifier, int CAIndex = -1);
    
    AIBoolean GetVectorOfBtColorFromIdentifier(vector<Bt::BtColor>& colors, string identifier, int CAIndex = -1);
    AIBoolean AddVectorOfBtColorToDictionary(vector<Bt::BtColor> colors, string identifier, int CAIndex = -1);
    AIBoolean AddBtColorToArrayInDictionary(Bt::BtColor color, string identifier, int CAIndex = -1);
    
    static string GetUIDStringForArt(AIArtHandle art);
private:
    AIDictionaryRef dictionary;
    
    AIDictKey Key(string identifier, int CAIndex = -1);
    //identifier -- the word part of the key... usually the variable name we're storing
    //CAIndex -- the crop area index
    //Returns the existing key, or a new key reference if none was found
    
    string GenerateUIDString(string identifier, int CAIndex = -1);
    //Returns a string in the format "identifier_CAIndex"
    
    AIUIDRef GetUIDRefFromIdentifier(string identifier, int CAIndex = -1);
    
    AIArtHandle GetArtHandleFromUIDRef(AIUIDRef uidRef);
    //Returns the handle to the art object with the associated id, or a nullptr handle if it couldn't be found
    ASBoolean CheckArtHandleFromUIDRef(AIUIDRef uidRef);
    //Returns TRUE if the uid points to an art handle in the document, FALSE if its nullptr or invalid
};

#endif /* DictionaryWriter_h */

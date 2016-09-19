//
//  DictionaryWriter.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/12/16.
//
//

#include "DictionaryWriter.h"

DictionaryWriter::DictionaryWriter()
{
    sAIDocument->GetDictionary(&dictionary);
}

DictionaryWriter::~DictionaryWriter()
{
    sAIDictionary->Release(dictionary);
}


AIDictKey DictionaryWriter::GenerateKey(string identifier, int CAIndex)
{
    AIDictKey currKey = NULL;
    currKey = sAIDictionary->Key(GenerateUIDString(identifier, CAIndex).c_str());
    
    return currKey;
}

string DictionaryWriter::GenerateUIDString(string identifier, int CAIndex)
{
    return identifier + "_" +  to_string(CAIndex);
}

AIUIDRef DictionaryWriter::GetUIDRefFromIdentifier(string identifier, int CAIndex)
{
    AIUIDRef uidForIdentifier = NULL;
    
    AIDictKey keyForIdentifier = GenerateKey(identifier, CAIndex);
    AIEntryRef entryForIdentifier = sAIDictionary->Get(dictionary, keyForIdentifier);
    
    if (entryForIdentifier)
    {
        AIUIDREFRef uidRefForIdentifier = NULL;
        sAIEntry->ToUIDREF(entryForIdentifier, &uidRefForIdentifier);
        if (uidRefForIdentifier)
        {
            sAIUIDREF->GetUID(uidRefForIdentifier, &uidForIdentifier);
            sAIUIDREF->Release(uidRefForIdentifier);
        }
        sAIEntry->Release(entryForIdentifier);
    }
    
    return uidForIdentifier;
}

AIArtHandle DictionaryWriter::GetArtHandleFromUIDRef(AIUIDRef uidRef)
{
    if (!uidRef)
    {
        return NULL;
    }
    
    AIArtHandle currArtObject = NULL;
    AIUIDREFRef uidREFRef = NULL;
    sAIUID->NewUIDREF(uidRef, &uidREFRef);
    
    sAIUIDUtils->GetReferencedArt(uidREFRef, &currArtObject);
    sAIUIDREF->Release(uidREFRef);
    
    if (currArtObject)
    {
        if (sAIArt->ValidArt(currArtObject, TRUE) )
        {
            return currArtObject;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }	
}

ASBoolean DictionaryWriter::CheckArtHandleFromUIDRef(AIUIDRef uidRef)
{
    //The handle was NULL, so we don't need to look for this one
    if (!uidRef)
    {
        return FALSE;
    }
    
    AIArtHandle currArtObject = GetArtHandleFromUIDRef(uidRef);
    
    if (currArtObject)
    {
        return sAIArt->ValidArt(currArtObject, TRUE);
    }
    else
    {
        return FALSE;
    }
}

AIReal DictionaryWriter::GetAIRealFromIdentifier(string identifier, int CAIndex)
{
    AIReal realValue = 0;
    
    AIDictKey dictKey = GenerateKey(identifier, CAIndex);
    if (sAIDictionary->IsKnown(dictionary, dictKey))
    {
        sAIDictionary->GetRealEntry(dictionary, dictKey, &realValue);
    }
    
    return realValue;
}

AIBoolean DictionaryWriter::AddAIRealToDictionary(AIReal value, string identifier, int CAIndex)
{
    AIDictKey dictKey = GenerateKey(identifier, CAIndex);
    if (dictKey)
    {
        sAIDictionary->SetRealEntry(dictionary, dictKey, value);
        return TRUE;
    }
    
    return FALSE;
}
AIBoolean DictionaryWriter::GetStringDataFromIdentifier(string& data, string identifier, int CAIndex)
{
    AIDictKey dictKey = GenerateKey(identifier, CAIndex);
    if (sAIDictionary->IsKnown(dictionary, dictKey))
    {
        ai::UnicodeString value;
        sAIDictionary->GetUnicodeStringEntry(dictionary, dictKey, value);
        data = value.as_Platform();
        return TRUE;
    }
    
    return FALSE;
}

AIBoolean DictionaryWriter::AddStringDataToDictionary(string data, string identifier, int CAIndex)
{
    AIDictKey dictKey = GenerateKey(identifier, CAIndex);
    if (dictKey)
    {
        ai::UnicodeString value(data);
        sAIDictionary->SetUnicodeStringEntry(dictionary, dictKey, value);
        return TRUE;
    }
    
    return FALSE;
}

AIArtHandle DictionaryWriter::GetArtHandleFromIdentifier(string identifier, int CAIndex)
{
    AIArtHandle foundArtHandle = NULL;
    
    AIUIDRef uidForIdentifier = GetUIDRefFromIdentifier(identifier, CAIndex);
    if (uidForIdentifier)
    {
        foundArtHandle = GetArtHandleFromUIDRef(uidForIdentifier);
        
        sAIUID->Release(uidForIdentifier);
    }
    
    return foundArtHandle;
}

AIBoolean DictionaryWriter::CheckDictionaryForArtObjectWithIdentifier(string identifier, int CAIndex)
{
    AIBoolean foundRef = FALSE;
    
    AIUIDRef uidForIdentifier = GetUIDRefFromIdentifier(identifier, CAIndex);
    if (uidForIdentifier)
    {
        foundRef = CheckArtHandleFromUIDRef(uidForIdentifier);
        sAIUID->Release(uidForIdentifier);
    }
    
    return foundRef;
}

AIBoolean DictionaryWriter::AddArtHandleToDictionary(AIArtHandle currArt, string identifier, int CAIndex)
{
    AIUIDRef uid = NULL;
    sAIUIDUtils->GetArtUID(currArt, TRUE /*To create one if there is none*/, &uid);
    
    if (uid)
    {
        AIUIDREFRef uidREFRef = NULL;
        sAIUID->NewUIDREF(uid, &uidREFRef);
        if (uidREFRef)
        {
            AIEntryRef newEntry = NULL;
            newEntry = sAIEntry->FromUIDREF(uidREFRef);
            if (newEntry)
            {
                sAIDictionary->Set(dictionary, GenerateKey(identifier, CAIndex), newEntry);
                sAIEntry->Release(newEntry);
            }
            sAIUIDREF->Release(uidREFRef);
        }
        sAIUID->Release(uid);
    }
    
    return TRUE;
}

AIBoolean DictionaryWriter::RemoveIdentifierFromDictionary(string identifier, int CAIndex)
{
    AIErr error = kNoErr;
    AIDictKey keyForIdentifier = GenerateKey(identifier, CAIndex);
    
    error = sAIDictionary->DeleteEntry(dictionary, keyForIdentifier);
    
    if (error == kNoErr)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

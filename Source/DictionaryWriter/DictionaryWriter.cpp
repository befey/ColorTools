//
//  DictionaryWriter.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/12/16.
//
//

#include "DictionaryWriter.h"
#include "GetIllustratorErrorCode.h"
#include <unordered_set>
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

DictionaryWriter::DictionaryWriter()
{
    sAIDocument->GetDictionary(&dictionary);
}

DictionaryWriter::DictionaryWriter(AIArtHandle art)
{
    sAIArt->GetDictionary(art, &dictionary);
}

DictionaryWriter::~DictionaryWriter()
{
    sAIDictionary->Release(dictionary);
}


AIDictKey DictionaryWriter::Key(string identifier, int CAIndex)
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
    
    AIDictKey keyForIdentifier = Key(identifier, CAIndex);
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
    
    ASErr error = sAIUIDUtils->GetReferencedArt(uidREFRef, &currArtObject);
    string err = GetIllustratorErrorCode(error);
    sAIUIDREF->Release(uidREFRef);
    
    return currArtObject;
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
    
    AIDictKey dictKey = Key(identifier, CAIndex);
    if (sAIDictionary->IsKnown(dictionary, dictKey))
    {
        sAIDictionary->GetRealEntry(dictionary, dictKey, &realValue);
    }
    
    return realValue;
}

AIBoolean DictionaryWriter::AddAIRealToDictionary(AIReal value, string identifier, int CAIndex)
{
    AIDictKey dictKey = Key(identifier, CAIndex);
    if (dictKey)
    {
        sAIDictionary->SetRealEntry(dictionary, dictKey, value);
        return TRUE;
    }
    
    return FALSE;
}
AIBoolean DictionaryWriter::GetStringDataFromIdentifier(string& data, string identifier, int CAIndex)
{
    AIDictKey dictKey = Key(identifier, CAIndex);
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
    AIDictKey dictKey = Key(identifier, CAIndex);
    if (dictKey)
    {
        ai::UnicodeString value(data);
        sAIDictionary->SetUnicodeStringEntry(dictionary, dictKey, value);
        return TRUE;
    }
    
    return FALSE;
}

AIBoolean DictionaryWriter::GetArrayDataFromIdentifier(AIArrayRef& data, string identifier, int CAIndex)
{
    AIDictKey dictKey = Key(identifier, CAIndex);
    if (sAIDictionary->IsKnown(dictionary, dictKey))
    {
        sAIDictionary->GetArrayEntry(dictionary, dictKey, &data);
        return TRUE;
    }
    
    return FALSE;
}

AIBoolean DictionaryWriter::AddArrayDataToDictionary(AIArrayRef data, string identifier, int CAIndex)
{
    AIDictKey dictKey = Key(identifier, CAIndex);
    if (dictKey)
    {
        sAIDictionary->SetArrayEntry(dictionary, dictKey, data);
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
        
        return foundArtHandle;
    }
    
    return NULL;
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
                sAIDictionary->Set(dictionary, Key(identifier, CAIndex), newEntry);
                sAIEntry->Release(newEntry);
            }
            sAIUIDREF->Release(uidREFRef);
        }
        sAIUID->Release(uid);
        return TRUE;
    }
    
    return FALSE;
}

AIBoolean DictionaryWriter::CheckDictionaryForIdentifier(string identifier, int CAIndex)
{
    return sAIDictionary->IsKnown(dictionary, Key(identifier, CAIndex));
}

AIBoolean DictionaryWriter::RemoveIdentifierFromDictionary(string identifier, int CAIndex)
{
    AIErr error = kNoErr;
    AIDictKey keyForIdentifier = Key(identifier, CAIndex);
    
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

AIBoolean DictionaryWriter::GetVectorOfAIArtHandleFromIdentifier(vector<AIArtHandle>& handles, string identifier, int CAIndex)
{
    AIArrayRef array = NULL;
    sAIArray->CreateArray(&array);
    AIBoolean result = GetArrayDataFromIdentifier(array, identifier, CAIndex);
    
    if (array)
    {
        int size = sAIArray->Size(array);
        for ( int i = 0; i < size; i++ )
        {
            AIEntryRef newEntry = NULL;
            newEntry = sAIArray->Get(array, i);
            if (newEntry)
            {
                AIUIDREFRef uidREFRef = NULL;
                sAIEntry->ToUIDREF(newEntry, &uidREFRef);
                
                if (uidREFRef)
                {
                    AIArtHandle ah = NULL;
                    sAIUIDUtils->GetReferencedArt(uidREFRef, &ah);
                    
                    if (ah)
                    {
                        handles.push_back(ah);
                    }
                    
                    sAIUIDREF->Release(uidREFRef);
                }
                
                sAIEntry->Release(newEntry);
            }
        }
        
        sAIArray->Release(array);
    }
    
    return result;
}

AIBoolean DictionaryWriter::AddVectorOfAIArtHandleToDictionary(vector<AIArtHandle> handles, string identifier, int CAIndex)
{
    AIArrayRef array;
    sAIArray->CreateArray(&array);
    
    for ( auto ah : handles )
    {
        AIUIDRef uid = NULL;
        AIErr err = sAIUIDUtils->GetArtUID(ah, TRUE /*To create one if there is none*/, &uid);
        string error = GetIllustratorErrorCode(err);
        
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
                    sAIArray->AppendEntry(array, newEntry);
                    sAIEntry->Release(newEntry);
                }
                sAIUIDREF->Release(uidREFRef);
            }
            sAIUID->Release(uid);
        }
    }
    
    AIBoolean result = AddArrayDataToDictionary(array, identifier, CAIndex);
    sAIArray->Release(array);
    
    return result;
}

AIBoolean DictionaryWriter::AddAIArtHandleToArrayInDictionary(AIArtHandle art, string identifier, int CAIndex)
{
    if (art != NULL)
    {
        vector<AIArtHandle> handles;
        GetVectorOfAIArtHandleFromIdentifier(handles, identifier, CAIndex);
        
        unordered_set<AIArtHandle> artSet;
        for ( auto handle : handles )
        {
            artSet.insert(handle);
        }
        artSet.insert(art);
        
        handles.clear();
        for ( auto handle : artSet )
        {
            handles.push_back(handle);
        }
        
        return AddVectorOfAIArtHandleToDictionary(handles, identifier, CAIndex);
    }
    return FALSE;
}

AIBoolean DictionaryWriter::GetVectorOfBtColorFromIdentifier(vector<BtColor>& colors, string identifier, int CAIndex)
{
    AIArrayRef mainArray = NULL;
    sAIArray->CreateArray(&mainArray);
    AIBoolean result = GetArrayDataFromIdentifier(mainArray, identifier, CAIndex);
    
    if (mainArray)
    {
        int size = sAIArray->Size(mainArray);
        for ( int i = 0; i < size; i++ )
        {
            AIEntryRef colorEntry = NULL;
            colorEntry = sAIArray->Get(mainArray, i);
            if (colorEntry)
            {
                AIArrayRef colorArray = NULL;
                
                sAIEntry->ToArray(colorEntry, &colorArray);
                
                if (colorArray)
                {
                    BtColor color;
                    
                    ai::UnicodeString serializedBtColor;
                    sAIArray->GetUnicodeStringEntry(colorArray, 0, serializedBtColor);
                    
                    std::istringstream is(serializedBtColor.as_Platform());
                    {
                        try
                        {
                            cereal::JSONInputArchive iarchive(is); // Create an input archive
                            iarchive(color);
                        }
                        catch (std::runtime_error e)
                        {
                            string s(e.what());
                        }
                    }
                    
                    if (color.AiColor().kind == kCustomColor)
                    {
                        AICustomColorHandle ccHandle = NULL;
                        AIEntryRef ccEntry = NULL;
                        ccEntry = sAIArray->Get(colorArray, 1);
                        sAIEntry->ToCustomColor(ccEntry, &ccHandle);
                        
                        color.AiCustomColorHandle(ccHandle);
                        
                        sAIEntry->Release(ccEntry);
                    }
                    
                    colors.push_back(color);
                    sAIArray->Release(colorArray);
                }
                
                sAIEntry->Release(colorEntry);
            }
        }
        
        sAIArray->Release(mainArray);
    }
    
    return result;
}

AIBoolean DictionaryWriter::AddVectorOfBtColorToDictionary(vector<BtColor> colors, string identifier, int CAIndex)
{
    AIArrayRef mainArray;
    sAIArray->CreateArray(&mainArray);
    
    for ( auto c : colors )
    {
        AIArrayRef colorArray;
        sAIArray->CreateArray(&colorArray);
        
        std::ostringstream os;
        {
            cereal::JSONOutputArchive oarchive(os); // Create an output archive
            oarchive(c);
        }
        
        AIEntryRef newEntry = NULL;
        newEntry = sAIEntry->FromUnicodeString(ai::UnicodeString(os.str()));
        if (newEntry)
        {
            sAIArray->AppendEntry(colorArray, newEntry);
            sAIEntry->Release(newEntry);
        }
        
        if (c.AiCustomColorHandle())
        {
            newEntry = sAIEntry->FromCustomColor(c.AiCustomColorHandle());
            if (newEntry)
            {
                sAIArray->AppendEntry(colorArray, newEntry);
                sAIEntry->Release(newEntry);
            }
        }
        
        newEntry = sAIEntry->FromArray(colorArray);
        if (newEntry)
        {
            sAIArray->AppendEntry(mainArray, newEntry);
            sAIEntry->Release(newEntry);
        }
        
        sAIArray->Release(colorArray);
    }
    
    AIBoolean result = AddArrayDataToDictionary(mainArray, identifier, CAIndex);
    sAIArray->Release(mainArray);
    
    return result;
}

AIBoolean DictionaryWriter::AddBtColorToArrayInDictionary(BtColor color, string identifier, int CAIndex)
{
    vector<BtColor> colors;
    GetVectorOfBtColorFromIdentifier(colors, identifier, CAIndex);
    
    colors.push_back(color);
    
    return AddVectorOfBtColorToDictionary(colors, identifier, CAIndex);
}

string DictionaryWriter::GetUIDStringForArt(AIArtHandle art)
{
    AIUIDREFRef uidREFRef = NULL;
    sAIUIDUtils->NewArtUIDREF(art, &uidREFRef);
    ai::UnicodeString name;
    sAIUIDREF->GetName(uidREFRef, name);
    return name.as_Platform();
}

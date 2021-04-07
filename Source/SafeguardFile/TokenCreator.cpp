//
//  TokenCreator.cpp
//  SafeguardTools
//
//  Created by T431962 on 12/2/16.
//
//

#include "TokenCreator.hpp"
#include "PlateNumber.h"
#include "ArtboardNameRetriever.hpp"
#include "CurrentFilenameRetriever.hpp"

using SafeguardFile::TokenCreator;

TokenCreator::TokenCreator(string plateNumber, int artboardIndex)
:
TokenCreator(plateNumber, ArtboardNameRetriever::GetName(artboardIndex, isDefaultName))
{}

TokenCreator::TokenCreator(string plateNumber, string artboardName)
{
    for (auto & c: plateNumber) c = toupper(c);
    this->plateNumber = plateNumber;
    
    for (auto & c: artboardName) c = toupper(c);
    this->artboardName = artboardName;
}

string TokenCreator::GetToken()
{
    if (artboardName.find(DEFAULT_ARTBOARD_NAME) != string::npos)
    {
        isDefaultName = true;
    }
    
    if (isDefaultName || artboardName == NO_TOKEN_DESIG)
    {
        return "";
    }
    else if ( PlateNumber(artboardName).IsValid() )
    {
        if (artboardName.find(CurrentFilenameRetriever::GetFilenameNoExt()) != string::npos )
        {
            return PlateNumber(artboardName).GetSuffix();
        }
    }
    
    return artboardName;
}

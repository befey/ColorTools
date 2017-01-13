//
//  FilePlacer.cpp
//  SafeguardTools
//
//  Created by T431962 on 1/13/17.
//
//

#include "FilePlacer.hpp"

using PlaceFileSearch::FilePlacer;

void FilePlacer::Place() const
{
    ai::FilePath aiFilePath((ai::UnicodeString)path);
    
    AIPlaceRequestData data;
    data.m_disableTemplate = false;
    data.m_doShowParamDialog = true;
    data.m_lParam = kPlacedArt;
    data.m_hNewArt = NULL;
    data.m_hOldArt = NULL;
    data.m_pFilePath = &aiFilePath;
    data.m_filemethod = 0;
    data.m_lPlaceMode = kVanillaPlace;
    data.m_PlaceTransformType = kAIPlaceTransformTypeNone;
    
    sAIPlaced->ExecPlaceRequest(data);
}

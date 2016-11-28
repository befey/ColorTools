//
//  BleedInfoController.cpp
//  SafeguardTools
//
//  Created by T431962 on 11/28/16.
//
//

#include "BleedInfoController.hpp"
#include "DictionaryWriter.h"
#include "BleedInfoPluginArtToArtboardMatcher.hpp"
#include "SafeguardJobFile.h"
#include "PlateBleedInfoUIController.hpp"
#include "BtDocumentView.hpp"

using PlateBleedInfo::BleedInfoController;

void BleedInfoController::HandleCropAreaNotification()
{
    if ( ShouldDoUpdate() )
    {
        SafeguardFile::SafeguardJobFile().UpdateBleedInfo();
        DictionaryWriter dw;
        dw.AddAIRealToDictionary(sAIArt->GetGlobalTimeStamp(), PLATE_BLEEDINFO_TIMESTAMP);
    }
}

void BleedInfoController::HandleCreateMenu()
{
    SafeguardFile::SafeguardJobFile().UpdateBleedInfo();
}

void BleedInfoController::HandleEditMenu()
{
    if ( !PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        SafeguardFile::SafeguardJobFile().UpdateBleedInfo();
    }
    
    BtDocumentView docView;
    docView.StoreCurrentDocumentView();
    
    PlateBleedInfoUIController().LoadExtension();
    sAICSXSExtension->LaunchExtension(PlateBleedInfoUIController::PLATEBLEEDINFO_UI_EXTENSION);
    //    sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Edit Safeguard Plate Info"), ai::UnicodeString("Redo Edit Safeguard Plate Info"));
}

bool BleedInfoController::ShouldDoUpdate()
{
    if (PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        size_t gTimeStamp = sAIArt->GetGlobalTimeStamp();
        DictionaryWriter dw;
        AIReal aTSDict = dw.GetAIRealFromIdentifier(PLATE_BLEEDINFO_TIMESTAMP);
        
        if ( gTimeStamp != aTSDict )
        {
            return true;
        }
    }
    return false;
}

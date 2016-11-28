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
        DrawBleedInfo();
    }
}

void BleedInfoController::HandleCreateMenu()
{
    DrawBleedInfo();
}

void BleedInfoController::HandleEditMenu()
{
    if ( !PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        DrawBleedInfo();
    }
    
    BtDocumentView docView;
    docView.StoreCurrentDocumentView();
    
    PlateBleedInfoUIController().LoadExtension();
    sAICSXSExtension->LaunchExtension(PlateBleedInfoUIController::PLATEBLEEDINFO_UI_EXTENSION);
    //    sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Edit Safeguard Plate Info"), ai::UnicodeString("Redo Edit Safeguard Plate Info"));
}

ASErr BleedInfoController::HandlePluginGroupNotify(AIPluginGroupMessage* message)
{
    if (strcmp( message->code, kAttachOperationCode ) == 0 && strcmp( message->time, kCheckOperationTime ) == 0)
    {
        return kNoErr;
    }
    if (strcmp( message->code, kAttachOperationCode ) == 0 && strcmp( message->time, kBeforeOperationTime ) == 0)
    {
        return kNoErr;
    }
    if (strcmp( message->code, kAttachOperationCode ) == 0 && strcmp( message->time, kAfterOperationTime ) == 0)
    {
        return kMarkValidPluginGroupReply;
    }
    return kUnhandledMsgErr;
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

void BleedInfoController::DrawBleedInfo()
{
    SafeguardFile::SafeguardJobFile().UpdateBleedInfo();
    DictionaryWriter dw;
    dw.AddAIRealToDictionary(sAIArt->GetGlobalTimeStamp(), PLATE_BLEEDINFO_TIMESTAMP);
}

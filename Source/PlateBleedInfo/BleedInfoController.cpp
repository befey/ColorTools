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
#include "BtArtHandle.hpp"
#include "RequiredLayersCreator.hpp"

static constexpr bool RedrawAllWithoutCheck = true;

using PlateBleedInfo::BleedInfoController;

BleedInfoController::BleedInfoController(vector<AINotifierHandle> notifiers)
: notifiers(notifiers)
{
    for (auto notifier : notifiers)
    {
        sAINotifier->SetNotifierActive(notifier, FALSE);
    }
}

BleedInfoController::~BleedInfoController()
{
    for (auto notifier : notifiers)
    {
        sAINotifier->SetNotifierActive(notifier, TRUE);
    }
}

void BleedInfoController::DeSelectAllPluginArts() const
{
    auto arts = PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().SortedPluginArts();
    for ( auto art : arts )
    {
        BtArtHandle(art.second).Selected(false);
    }
}

void BleedInfoController::RedrawOnDocOpen()
{
    if ( PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        DrawBleedInfo(RedrawAllWithoutCheck);
    }
}

void BleedInfoController::HandleSelectionChangeNotification()
{
    if ( PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        if ( !SameTimestamp() )
        {
            DrawBleedInfo();
        }
    }
}

void BleedInfoController::HandleCropAreaNotification()
{
    if ( PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        DrawBleedInfo(RedrawAllWithoutCheck);
    }
}

void BleedInfoController::HandleCreateMenu()
{
    RequiredLayersCreator::CreateRequiredLayers();
    DrawBleedInfo(RedrawAllWithoutCheck);
}

void BleedInfoController::HandleEditMenu()
{
    if ( !PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        DrawBleedInfo(RedrawAllWithoutCheck);
        sAIUndo->SetUndoTextUS(ai::UnicodeString("Undo Edit Safeguard Plate Info"), ai::UnicodeString("Redo Edit Safeguard Plate Info"));
    }
    
    BtDocumentView docView;
    docView.StoreCurrentDocumentView();
    
    PlateBleedInfoUIController().LoadExtension();
    sAICSXSExtension->LaunchExtension(PlateBleedInfoUIController::PLATEBLEEDINFO_UI_EXTENSION);
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
        return kUnhandledMsgErr; //kNoErr; //kMarkValidPluginGroupReply;
    }
    return kUnhandledMsgErr;
}

ASErr BleedInfoController::HandlePluginGroupUpdate(AIPluginGroupMessage* message)
{
    if (!SameTimestamp())
    {
        DrawBleedInfo();  //message->art
    }
    
    return kNoErr;
}

bool BleedInfoController::SameTimestamp()
{
    if (PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher().IsBleedInfoPluginArtCreated() )
    {
        size_t gTimeStamp = sAIArt->GetGlobalTimeStamp();
        DictionaryWriter dw;
        AIReal aTSDict = dw.GetAIRealFromIdentifier(PLATE_BLEEDINFO_TIMESTAMP);
        
        if ( gTimeStamp == aTSDict )
        {
            return true;
        }
    }
    return false;
}

void BleedInfoController::DrawBleedInfo(bool redrawAllWithoutCheck)
{
    if (! sAIIsolationMode->IsInIsolationMode() )
    {
        SafeguardFile::SafeguardJobFile(redrawAllWithoutCheck).UpdateBleedInfo();
        DictionaryWriter dw;
        dw.AddAIRealToDictionary(sAIArt->GetGlobalTimeStamp(), PLATE_BLEEDINFO_TIMESTAMP);
    }
}

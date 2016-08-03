//
//  PlateBleedInfoUIController.cpp
//  SafeguardTools
//
//  Created by T431962 on 8/3/16.
//
//

#include "AppContext.hpp"
#include "PlateBleedInfoUIController.hpp"
#include "SafeguardToolsPlugin.h"
#include "SafeguardToolsSuites.h"

using SafeguardFile::PlateBleedInfoUIController;
using SafeguardFile::SafeguardJobFile;

PlateBleedInfoUIController::PlateBleedInfoUIController(void)
: FlashUIController(PLATEBLEEDINFO_UI_EXTENSION)
{
}


/* Add event listeners
 */
csxs::event::EventErrorCode PlateBleedInfoUIController::RegisterCSXSEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
//        result =  fPPLib.AddEventListener(EVENT_TYPE_MAKEPDF_CLICKED, MakePdfButtonClickedFunc, this);
//        if (result != csxs::event::kEventErrorCode_Success)
//        {
//            break;
//        }
    }
    while(false);
    return result;
}

/* Remove event listeners
 */
csxs::event::EventErrorCode PlateBleedInfoUIController::RemoveEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
//        result =  fPPLib.RemoveEventListener(EVENT_TYPE_MAKEPDF_CLICKED, MakePdfButtonClickedFunc, this);
//        if (result != csxs::event::kEventErrorCode_Success)
//        {
//            break;
//        }
    }
    while(false);
    return result;
}

ASErr PlateBleedInfoUIController::SendData()
{
    AIErr error = kNoErr;
    return error;
}

/* Parse received data from flash panel.
 */
void PlateBleedInfoUIController::ParseData(const char* eventData)
{
    return;
}

void PlateBleedInfoUIController::EditBleedInfo(SafeguardJobFile sgJobFile)
{
    sAICSXSExtension->LaunchExtension(SafeguardFile::PlateBleedInfoUIController::PLATEBLEEDINFO_UI_EXTENSION);
}

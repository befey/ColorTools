//
//  PrintToPdfUIController.cpp
//  SafeguardTools
//
//  Created by T431962 on 4/15/16.
//
//

#include "PrintToPdfUIController.h"


PrintToPdfUIController::PrintToPdfUIController(void)
: FlashUIController(PRINTTOPDF_UI_EXTENSION)
{
}


/* Add event listeners
 */
csxs::event::EventErrorCode PrintToPdfUIController::RegisterCSXSEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        //result =  fPPLib.AddEventListener(EVENT_TYPE_CHANGE_CLICKED, ChangeButtonClickedFunc, this);
        //if (result != csxs::event::kEventErrorCode_Success)
        //{
        //    break;
        //}
    }
    while(false);
    return result;
}

/* Remove event listeners
 */
csxs::event::EventErrorCode PrintToPdfUIController::RemoveEventListeners()
{
    csxs::event::EventErrorCode result = csxs::event::kEventErrorCode_Success;
    do {
        //result =  fPPLib.RemoveEventListener(EVENT_TYPE_CHANGE_CLICKED, ChangeButtonClickedFunc, this);
        //if (result != csxs::event::kEventErrorCode_Success)
        //{
        //    break;
        //}
    }
    while(false);
    return result;
}

ASErr PrintToPdfUIController::SendData()
{
    AIErr error = kNoErr;
    return error;
}

/* Parse received data from flash panel.
 */
void PrintToPdfUIController::ParseData(const char* eventData)
{
    return;
}

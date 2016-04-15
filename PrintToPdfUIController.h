//
//  PrintToPdfUIController.h
//  SafeguardTools
//
//  Created by T431962 on 4/15/16.
//
//

#ifndef __SafeguardTools__PrintToPdfUIController__
#define __SafeguardTools__PrintToPdfUIController__

#include "SDKPlugPlug.h"
#include "FlashUIController.h"

#define PRINTTOPDF_UI_EXTENSION             "com.gosafeguard.SafeguardTools.PrintToPdf"
#define ILST_APP                            "ILST"

class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;

class PrintToPdfUIController : public FlashUIController
{
public:
    PrintToPdfUIController(void);
    
    /** Registers the events this plug-in will listen for with PlugPlug.
     @return error code if error found, or success otherwise.
     */
    csxs::event::EventErrorCode RegisterCSXSEventListeners();
    
    /** Removes the previously added event listeners from PlugPlug.
     @return error code if error found, or success otherwise.
     */
    csxs::event::EventErrorCode RemoveEventListeners();
    
    ASErr SendData();
    
    void ParseData(const char* eventData);
        
private:
};

#endif /* defined(__SafeguardTools__PrintToPdfUIController__) */

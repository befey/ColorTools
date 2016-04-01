//
//  BtAiMenuItem.h
//  SafeguardTools
//
//  Created by T431962 on 3/28/16.
//
//

#ifndef __SafeguardTools__BtAiMenuItem__
#define __SafeguardTools__BtAiMenuItem__

#include "BtAiMenuItemHandles.h"

extern "C"  AIMenuSuite *sAIMenu;

class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;

class BtAiMenuItem {
public:
    BtAiMenuItem(AIPlatformAddMenuItemDataUS, ai::int32 opts);
    
    static void AddMenu(BtAiMenuItem, BtAiMenuItemHandles*);
    
    void SetMenuItemData(AIPlatformAddMenuItemDataUS);
    void AddSubMenuItem(BtAiMenuItem);
    void SetOptions(ai::int32);

private:
    AIPlatformAddMenuItemDataUS menuItemData;
    vector<BtAiMenuItem> subMenuItems;
    ai::int32 options;
    
    bool IsMenuGroup();
    
    static bool SDKGroupAlreadyMade(ai::UnicodeString menuGroup);

};

#endif /* defined(__SafeguardTools__BtAiMenuItem__) */

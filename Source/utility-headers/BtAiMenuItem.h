//
//  BtAiMenuItem.h
//  SafeguardTools
//
//  Created by T431962 on 3/28/16.
//
//

#ifndef __SafeguardTools__BtAiMenuItem__
#define __SafeguardTools__BtAiMenuItem__


//=================================
// included dependencies
#include "BtAiMenuItemHandles.h"


//=================================
// forward declared dependencies
extern AIMenuSuite *sAIMenu;

class SafeguardToolsPlugin;
extern SafeguardToolsPlugin *gPlugin;


//=================================
// BtAiMenuItem - wrapper for a Menu creation functions
class BtAiMenuItem {
public:
    BtAiMenuItem(string menuItemTitle, ai::int32 opts);
    BtAiMenuItem(string groupName, string menuItemTitle, ai::int32 opts);
    
    static void AddMenu(BtAiMenuItem, BtAiMenuItemHandles*);
    
    void AddSubMenuItem(BtAiMenuItem);
    BtAiMenuItem& SetOptions(ai::int32);
    BtAiMenuItem& SetAutoUpdateOptions(ai::int32 action,
                              ai::int32 ifObjectIsInArtwork,
                              ai::int32 ifObjectIsNotInArtwork,
                              ai::int32 ifObjectIsSelected,
                              ai::int32 ifObjectIsNotSelected,
                              ai::int32 ifIsTrue,
                              ai::int32 ifIsNotTrue);

private:
    string groupName;
    string menuItemTitle;
    vector<BtAiMenuItem> subMenuItems;
    ai::int32 options;
    
    struct AutoUpdateOptions {
        ai::int32 action = 0;
        ai::int32 ifObjectIsInArtwork = 0;
        ai::int32 ifObjectIsNotInArtwork = 0;
        ai::int32 ifObjectIsSelected = 0;
        ai::int32 ifObjectIsNotSelected = 0;
        ai::int32 ifIsTrue = 0;
        ai::int32 ifIsNotTrue = 0;
    } autoUpdateOptions;
    
    bool wantsAutoUpdate = false;
    
    bool IsMenuGroup();
    
    static bool SDKGroupAlreadyMade(string menuGroup);
    
    AIPlatformAddMenuItemDataUS GetMenuItemData();
};

#endif /* defined(__SafeguardTools__BtAiMenuItem__) */

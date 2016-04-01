//
//  BtAiMenuItem.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/28/16.
//
//

#include "BtAiMenuItem.h"
#include "SafeguardToolsPlugin.h"


BtAiMenuItem::BtAiMenuItem(AIPlatformAddMenuItemDataUS data, ai::int32 opts)
{
    SetMenuItemData(data);
    SetOptions(opts);
}

void BtAiMenuItem::SetMenuItemData(AIPlatformAddMenuItemDataUS data)
{
    menuItemData.groupName = data.groupName;
    menuItemData.itemText = data.itemText;
}

void BtAiMenuItem::AddSubMenuItem(BtAiMenuItem subMenuItem)
{
    subMenuItems.push_back(subMenuItem);
}

void BtAiMenuItem::SetOptions(ai::int32 o)
{
    options = o;
}



void BtAiMenuItem::AddMenu(BtAiMenuItem menuItem, BtAiMenuItemHandles* addedMenuItems)
{
    string t = menuItem.menuItemData.itemText.getInStdString(kAIPlatformCharacterEncoding);
    const char* c = t.c_str();
    
    if (menuItem.IsMenuGroup()) {
        if ( !SDKGroupAlreadyMade(menuItem.menuItemData.itemText) ) {
            AIMenuItemHandle dummyItem;
            AIMenuGroup dummyGroup;

            sAIMenu->AddMenuItem( gPlugin->GetPluginRef(), c, &menuItem.menuItemData, kMenuItemNoOptions, &dummyItem );
            
            sAIMenu->AddMenuGroupAsSubMenu( c, menuItem.options, dummyItem, &dummyGroup );
        }
        for (auto i: menuItem.subMenuItems)
        {
            AddMenu(i, addedMenuItems);
        }
    }
    else
    {
        AIMenuItemHandle dummyHandle;
        sAIMenu->AddMenuItem( gPlugin->GetPluginRef(), c, &menuItem.menuItemData, menuItem.options, &dummyHandle );
        addedMenuItems->AddHandle(t, dummyHandle);
    }
}

bool BtAiMenuItem::IsMenuGroup()
{
    if (subMenuItems.empty()) {
        return false;
    }
    return true;
}


bool BtAiMenuItem::SDKGroupAlreadyMade(ai::UnicodeString menuGroup) {
    AIMenuGroup dummyGroup;
    const char* cstr = menuGroup.as_Platform().c_str();
    
    int count;
    sAIMenu->CountMenuGroups( &count );
    while (count > 0)
    {
        sAIMenu->GetNthMenuGroup( count-1, &dummyGroup );
        const char *name;
        sAIMenu->GetMenuGroupName( dummyGroup, &name );
        if ( std::strcmp(name, cstr ) == 0 )
        {
            return true;
        }
        count--;
    }
    return false;
}

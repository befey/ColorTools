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

void BtAiMenuItem::SetAutoUpdateOptions(ai::int32 action, ai::int32 ifObjectIsInArtwork, ai::int32 ifObjectIsNotInArtwork, ai::int32 ifObjectIsSelected, ai::int32 ifObjectIsNotSelected, ai::int32 ifIsTrue, ai::int32 ifIsNotTrue)
{
    autoUpdateOptions.action = action;
    autoUpdateOptions.ifObjectIsInArtwork = ifObjectIsInArtwork;
    autoUpdateOptions.ifObjectIsNotInArtwork = ifObjectIsNotInArtwork;
    autoUpdateOptions.ifObjectIsSelected = ifObjectIsSelected;
    autoUpdateOptions.ifObjectIsNotSelected = ifObjectIsNotSelected;
    autoUpdateOptions.ifIsTrue = ifIsTrue;
    autoUpdateOptions.ifIsNotTrue = ifIsNotTrue;
    wantsAutoUpdate = true;
}



void BtAiMenuItem::AddMenu(BtAiMenuItem menuItem, BtAiMenuItemHandles* addedMenuItems)
{
    string t = menuItem.menuItemData.itemText.getInStdString(kAIPlatformCharacterEncoding);
    const char* c = t.c_str();
    
    if (menuItem.IsMenuGroup())
    {
        if ( !SDKGroupAlreadyMade(menuItem.menuItemData.itemText) )
        {
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
        if (menuItem.wantsAutoUpdate)
        {
            sAIMenu->UpdateMenuItemAutomatically(dummyHandle,
                                                 menuItem.autoUpdateOptions.action,
                                                 menuItem.autoUpdateOptions.ifObjectIsInArtwork,
                                                 menuItem.autoUpdateOptions.ifObjectIsNotInArtwork,
                                                 menuItem.autoUpdateOptions.ifObjectIsSelected,
                                                 menuItem.autoUpdateOptions.ifObjectIsNotSelected,
                                                 menuItem.autoUpdateOptions.ifIsTrue,
                                                 menuItem.autoUpdateOptions.ifIsNotTrue);

        }
        addedMenuItems->AddHandle(t, dummyHandle);
    }
}

bool BtAiMenuItem::IsMenuGroup()
{
    if (subMenuItems.empty())
    {
        return false;
    }
    return true;
}


bool BtAiMenuItem::SDKGroupAlreadyMade(ai::UnicodeString menuGroup)
{
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

//
//  BtAiMenuItem.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/28/16.
//
//

#include "BtAiMenuItem.h"
#include "SafeguardToolsPlugin.h"

BtAiMenuItem::BtAiMenuItem(string menuItemTitle, ai::int32 opts)
:
BtAiMenuItem("", menuItemTitle, opts)
{}

BtAiMenuItem::BtAiMenuItem(string groupName, string menuItemTitle, ai::int32 opts)
:
groupName(groupName), menuItemTitle(menuItemTitle), options(opts)
{}

AIPlatformAddMenuItemDataUS BtAiMenuItem::GetMenuItemData()
{
    return {groupName.c_str(), ai::UnicodeString(menuItemTitle)};
}

void BtAiMenuItem::AddSubMenuItem(BtAiMenuItem subMenuItem)
{
    if (subMenuItem.groupName == "")
    {
        subMenuItem.groupName = this->menuItemTitle;
    }
    subMenuItems.push_back(subMenuItem);
}

BtAiMenuItem& BtAiMenuItem::SetOptions(ai::int32 o)
{
    options = o;
    return *this;
}

BtAiMenuItem& BtAiMenuItem::SetAutoUpdateOptions(ai::int32 action, ai::int32 ifObjectIsInArtwork, ai::int32 ifObjectIsNotInArtwork, ai::int32 ifObjectIsSelected, ai::int32 ifObjectIsNotSelected, ai::int32 ifIsTrue, ai::int32 ifIsNotTrue)
{
    autoUpdateOptions.action = action;
    autoUpdateOptions.ifObjectIsInArtwork = ifObjectIsInArtwork;
    autoUpdateOptions.ifObjectIsNotInArtwork = ifObjectIsNotInArtwork;
    autoUpdateOptions.ifObjectIsSelected = ifObjectIsSelected;
    autoUpdateOptions.ifObjectIsNotSelected = ifObjectIsNotSelected;
    autoUpdateOptions.ifIsTrue = ifIsTrue;
    autoUpdateOptions.ifIsNotTrue = ifIsNotTrue;
    wantsAutoUpdate = true;
    
    return *this;
}



void BtAiMenuItem::AddMenu(BtAiMenuItem menuItem, BtAiMenuItemHandles* addedMenuItems)
{
    AIPlatformAddMenuItemDataUS data = menuItem.GetMenuItemData();
    if (menuItem.IsMenuGroup())
    {
        if ( !SDKGroupAlreadyMade(menuItem.menuItemTitle) )
        {
            AIMenuItemHandle dummyItem;
            AIMenuGroup dummyGroup;

            sAIMenu->AddMenuItem( gPlugin->GetPluginRef(), menuItem.menuItemTitle.c_str(), &data, kMenuItemNoOptions, &dummyItem );
            
            sAIMenu->AddMenuGroupAsSubMenu( menuItem.menuItemTitle.c_str(), menuItem.options, dummyItem, &dummyGroup );
        }
        for (auto i: menuItem.subMenuItems)
        {
            AddMenu(i, addedMenuItems);
        }
    }
    else
    {
        AIMenuItemHandle dummyHandle;
        sAIMenu->AddMenuItem( gPlugin->GetPluginRef(), menuItem.menuItemTitle.c_str(), &data, menuItem.options, &dummyHandle );
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
        addedMenuItems->AddHandle(menuItem.menuItemTitle, dummyHandle);
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


bool BtAiMenuItem::SDKGroupAlreadyMade(string menuGroup)
{
    AIMenuGroup dummyGroup;
    const char* cstr = menuGroup.c_str();
    
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

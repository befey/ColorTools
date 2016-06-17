//
//  BtAiMenuItemHandles.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/29/16.
//
//

#include "BtAiMenuItemHandles.h"

void BtAiMenuItemHandles::AddHandle(string key, AIMenuItemHandle value)
{
    menuItemHandles.insert({key, value});
}

AIMenuItemHandle BtAiMenuItemHandles::GetHandleWithKey(string key)
{
    return menuItemHandles[key];
}

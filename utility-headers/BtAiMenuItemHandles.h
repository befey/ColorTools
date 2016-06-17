//
//  BtAiMenuItemHandles.h
//  SafeguardTools
//
//  Created by T431962 on 3/29/16.
//
//

#ifndef __SafeguardTools__BtAiMenuItemHandles__
#define __SafeguardTools__BtAiMenuItemHandles__

//=================================
// included dependencies
#include <unordered_map>

//=================================
// forward declared dependencies


//=================================
// BtAiMenuItemHandles - manages the list of added menu item handles
class BtAiMenuItemHandles
{
public:
    void AddHandle(string, AIMenuItemHandle);
    AIMenuItemHandle GetHandleWithKey(string);

private:
    unordered_map<string, AIMenuItemHandle> menuItemHandles;
};

#endif /* defined(__SafeguardTools__BtAiMenuItemHandles__) */

//
//  BtColor.cpp
//  SafeguardTools
//
//  Created by T431962 on 2/16/15.
//
//

#include "BtColor.h"

#define TMPBLACKNAME "CustomBlack"
#define TMPWHITENAME "CustomWhite"


//Constructor
BtColor::BtColor(string name, AICustomColorTag kind, AICustomColorUnion c, AICustomColorFlags flag) : pName(name), pKind(kind), pC(c), pFlag (flag)
{

}

BtColor::BtColor() : BtColor("", kCustomFourColor, {}, 0) {}

//Getters/Setters

std::string BtColor::GetName() const
{
    return pName;
}
AICustomColorTag BtColor::GetKind() const
{
    return pKind;
}
AICustomColorUnion BtColor::GetCustomColorUnion() const
{
    return pC;
}
AICustomColorFlags BtColor::GetCustomColorFlags() const
{
    return pFlag;
}


//Behaviors

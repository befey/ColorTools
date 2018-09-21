//
//  ColorListCommand.hpp
//  SafeguardTools
//
//  Created by T431962 on 3/9/17.
//
//

#ifndef ColorListCommand_hpp
#define ColorListCommand_hpp

#include "IBtCommand.hpp"
#include "BtColor.h"

class ColorListCommand : public IBtCommand
{
protected:
    ColorListCommand(vector<Bt::BtColor>& colorList) : colorList(colorList) {};
    ~ColorListCommand() {};
    
    vector<Bt::BtColor>& colorList;
};


class CleanUpColorListCommand : public ColorListCommand
{
public:
    CleanUpColorListCommand(vector<Bt::BtColor>& colorList);
    
    bool Execute() const override;

private:    
    vector<shared_ptr<IBtCommand>> commands;
};


class RemoveNonSolidColorsCommand : public ColorListCommand
{
public:
    RemoveNonSolidColorsCommand(vector<Bt::BtColor>& colorList) : ColorListCommand(colorList) {};
    
    bool Execute() const override;
};

class RemoveNonPrintingColorsCommand : public ColorListCommand
{
public:
    RemoveNonPrintingColorsCommand(vector<Bt::BtColor>& colorList) : ColorListCommand(colorList) {};
    
    bool Execute() const override;
};


class RemoveDuplicateColorsCommand : public ColorListCommand
{
public:
    RemoveDuplicateColorsCommand(vector<Bt::BtColor>& colorList) : ColorListCommand(colorList) {};
    
    bool Execute() const override;
};

class RemoveBlackIfCMYKPresentCommand : public ColorListCommand
{
public:
    RemoveBlackIfCMYKPresentCommand(vector<Bt::BtColor>& colorList) : ColorListCommand(colorList) {};
    
    bool Execute() const override;
};



#endif /* ColorListCommand_hpp */

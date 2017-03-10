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
    ColorListCommand(vector<BtColor>& colorList) : colorList(colorList) {};
    ~ColorListCommand() {};
    
    vector<BtColor>& colorList;
};


class CleanUpColorListCommand : public ColorListCommand
{
public:
    CleanUpColorListCommand(vector<BtColor>& colorList);
    
    void Execute() const override;

private:    
    vector<shared_ptr<IBtCommand>> commands;
};


class RemoveNonSolidColorsCommand : public ColorListCommand
{
public:
    RemoveNonSolidColorsCommand(vector<BtColor>& colorList) : ColorListCommand(colorList) {};
    
    void Execute() const override;
};

class RemoveNonPrintingColorsCommand : public ColorListCommand
{
public:
    RemoveNonPrintingColorsCommand(vector<BtColor>& colorList) : ColorListCommand(colorList) {};
    
    void Execute() const override;
};


class RemoveDuplicateColorsCommand : public ColorListCommand
{
public:
    RemoveDuplicateColorsCommand(vector<BtColor>& colorList) : ColorListCommand(colorList) {};
    
    void Execute() const override;
};

class RemoveBlackIfCMYKPresentCommand : public ColorListCommand
{
public:
    RemoveBlackIfCMYKPresentCommand(vector<BtColor>& colorList) : ColorListCommand(colorList) {};
    
    void Execute() const override;
};



#endif /* ColorListCommand_hpp */

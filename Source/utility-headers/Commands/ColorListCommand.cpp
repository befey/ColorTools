//
//  ColorListCommand.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/9/17.
//
//

#include "ColorListCommand.hpp"
#include "ColorListChecker.hpp"

CleanUpColorListCommand::CleanUpColorListCommand(vector<BtColor>& colorList)
:
ColorListCommand(colorList)
{
    commands.push_back(make_shared<RemoveNonSolidColorsCommand>(colorList));
    commands.push_back(make_shared<RemoveDuplicateColorsCommand>(colorList));
    commands.push_back(make_shared<RemoveBlackIfCMYKPresentCommand>(colorList));
    commands.push_back(make_shared<RemoveNonPrintingColorsCommand>(colorList));
}

void CleanUpColorListCommand::Execute() const
{
    for ( auto command : commands )
    {
        command->Execute();
    }
}

void RemoveNonSolidColorsCommand::Execute() const
{
    if ( colorList.empty() )
    {
        return;
    }
    for ( auto color = colorList.begin(); color != colorList.end(); color++ )
    {
        if ( !colorList.empty() )
        {
            ColorListNonSolidColorChecker checker(*color);
            colorList.erase( std::remove_if(color, colorList.end(), checker ), colorList.end() );
        }
    }
}

void RemoveNonPrintingColorsCommand::Execute() const
{
    colorList.erase(
                      std::remove_if(colorList.begin(), colorList.end(), [this](BtColor c)
                                     {
                                         if (c.IsNonPrinting())
                                         {
                                             return true;
                                         }
                                         else if ((c.Name() == SafeguardFile::REGISTRATION_COLOR_NAME) && (colorList.size() > 1))
                                         {
                                             return true;
                                         }
                                         return false;
                                     }
                                     ),
                      colorList.end()
                      );
}

void RemoveDuplicateColorsCommand::Execute() const
{
    if ( colorList.size() <= 1 )
    {
        return;
    }
    for ( auto color = colorList.begin(); color != colorList.end(); color++ )
    {
        if ( !colorList.empty() )
        {
            ColorListDuplicateChecker checker(*color);
            colorList.erase( std::remove_if(color, colorList.end(), checker ), colorList.end() );
        }
    }
}

void RemoveBlackIfCMYKPresentCommand::Execute() const
{
    for ( auto color : colorList )
    {
        if (color.PrintsAsProcess())
        {
            colorList.erase(
                              std::remove_if(colorList.begin(), colorList.end(), [](BtColor c)
                                             {
                                                 if (c.IsBlack(false))
                                                 {
                                                     return true;
                                                 }
                                                 return false;
                                             }
                                             ),
                              colorList.end()
                              );
            break;
        }
    }
}

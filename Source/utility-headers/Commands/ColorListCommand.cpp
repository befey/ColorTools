//
//  ColorListCommand.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/9/17.
//
//

#include "ColorListCommand.hpp"
#include "ColorListChecker.hpp"

CleanUpColorListCommand::CleanUpColorListCommand(vector<Bt::BtColor>& colorList)
:
ColorListCommand(colorList)
{
    commands.push_back(make_shared<RemoveDuplicateColorsCommand>(colorList));
    commands.push_back(make_shared<RemoveNonSolidColorsCommand>(colorList));
    commands.push_back(make_shared<RemoveBlackIfCMYKPresentCommand>(colorList));
    commands.push_back(make_shared<RemoveNonPrintingColorsCommand>(colorList));
}

bool CleanUpColorListCommand::Execute() const
{
    for ( auto command : commands )
    {
        command->Execute();
    }
    
    return true;
}

bool RemoveNonSolidColorsCommand::Execute() const
{
    if ( colorList.empty() )
    {
        return false;
    }
    for ( auto color = colorList.begin(); color != colorList.end(); color++ )
    {
        if ( !colorList.empty() )
        {
            ColorListNonSolidColorChecker checker(*color);
            colorList.erase( std::remove_if(color, colorList.end(), checker ), colorList.end() );
        }
    }
    
    return true;
}

bool RemoveNonPrintingColorsCommand::Execute() const
{
    colorList.erase(
                      std::remove_if(colorList.begin(), colorList.end(), [this](Bt::BtColor c)
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
    return true;
}

bool RemoveDuplicateColorsCommand::Execute() const
{
    if ( colorList.size() <= 1 )
    {
        return false;
    }
    for ( auto color = colorList.begin(); color != colorList.end(); color++ )
    {
        if ( !colorList.empty() )
        {
            ColorListDuplicateChecker checker(*color);
            colorList.erase( std::remove_if(color, colorList.end(), checker ), colorList.end() );
        }
    }
    
    return true;
}

bool RemoveBlackIfCMYKPresentCommand::Execute() const
{
    for ( auto color : colorList )
    {
        if (color.PrintsAsProcess())
        {
            colorList.erase(
                            std::remove_if(colorList.begin(), colorList.end(), [](Bt::BtColor c)
                                           {
                                               if (c.IsBlack(DoNotIncludeCMYKBuilds))
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
    
    return true;
}

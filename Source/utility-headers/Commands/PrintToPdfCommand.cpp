//
//  PrintToPdfCommand.cpp
//  SafeguardTools
//
//  Created by T431962 on 3/10/17.
//
//

#include "PrintToPdfCommand.hpp"
#include "TypeToPathsConverter.hpp"
#include "LayerVisibility.hpp"

bool PrintToPdfCommand::Execute() const
{
    for ( auto command : commands )
    {
        if ( !command->Execute() && command->RequireSuccess() )
        {
            return false;
        }
    }
    
    return true;
}


bool ConvertTypeToPathsCommand::Execute() const
{
    PrintToPdf::TypeToPathsConverter().ConvertTypeToPaths();
    return true;
}

bool SetLayerVisibilityCommand::Execute() const
{
    PrintToPdf::LayerVisibility::GetLayerVisibility(productType, preset)->SetLayerVisibility();
    
    return true;
}

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
#include "BleedInfoPluginArtToArtboardMatcher.hpp"
#include "DictionaryWriter.h"
#include "TickMarkDrawer.h"
#include "TickMarkSettings.hpp"
#include "BtArtHandle.hpp"

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

bool MakeKeylineBoundingBox::Execute() const
{
    PlateBleedInfo::BleedInfoPluginArtToArtboardMatcher matcher;
    
    if (matcher.IsBleedInfoPluginArtCreated())
    {
        SafeguardFile::TickMarkDrawer drawer{SafeguardFile::TickMarkSettings{}};
        for (auto bleedInfoArt : matcher.SortedPluginArts())
        {
            AIArtHandle resultArt = nullptr;
            sAIPluginGroup->GetPluginArtResultArt(bleedInfoArt.second, &resultArt);
            AIArtHandle tickMarkArt = DictionaryWriter(resultArt).GetArtHandleFromIdentifier(drawer.GetDictionaryLabel(resultArt));
            BtArtHandle(tickMarkArt).VisitEachArtInTree( [](BtArtHandle handle)
            {
                if (handle.StrokeColor().IsRegistration())
                {
                    handle.StrokeColor(Bt::BtStandardColors().Keyline());
                }
            });
        }
    }
    
    return true;
}

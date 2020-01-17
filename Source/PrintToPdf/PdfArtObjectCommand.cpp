//
//  PdfArtObjectCommand.cpp
//  SafeguardTools
//
//  Created by T431962 on 2/12/18.
//
//

#include "PdfArtObjectCommand.hpp"
#include "ArtObjectCommand.hpp"
#include "BtColor.h"
#include "ArtObjectFilter.hpp"
#include "ArtTree.h"
#include "BtArtHandle.hpp"


using PrintToPdf::PdfArtObjectCommand;
using PrintToPdf::ManufacturingPdfArtObjectCommand;


bool ManufacturingPdfArtObjectCommand::Execute() const
{
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    
    AIArtSpec allSpecs[] = { { kPathArt , 0 , 0 },
        { kCompoundPathArt , 0 , 0 },
        { kPlacedArt , 0 , 0 },
        { kRasterArt , 0 , 0 },
        { kPluginArt , 0 , 0 },
        { kMeshArt , 0 , 0 },
        { kTextFrameArt , 0 , 0 },
        { kSymbolArt , 0 , 0 },
    };
    sAIArtSet->MatchingArtSet( allSpecs , 8, artSet );
    
    Bt::BtStandardColors stdColors;
    vector<Bt::BtColor> colors = {stdColors.NumberingRed(), stdColors.NumberingBlack()};
    ArtObjectColorFilter filter(&artSet, colors );
    filter.Execute();
    
    ProcessArtSet(artSet, [](AIArtHandle handle)
                  {
                      BtArtHandle btHandle(handle);
                      
                      if (!btHandle.Hidden())
                      {
                          btHandle.Locked(false);
                          
                          btHandle.Dispose();
                      }
                  });
    
    sAIArtSet->DisposeArtSet(&artSet);
    
    return true;
}

bool PdfArtObjectCommand::Execute() const
{
    for ( auto command : commands )
    {
        command->Execute();
    }
    
    return true;
}

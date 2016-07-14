//
//  TypeToPathsConverter.cpp
//  SafeguardTools
//
//  Created by T431962 on 7/13/16.
//
//

#include "TypeToPathsConverter.hpp"
#include "ArtTree.h"
#include "Plate.h"

using PrintToPdf::TypeToPathsConverter;

bool TypeToPathsConverter::ConvertTypeToPaths() const
{
    AIArtSet artSet;
    sAIArtSet->NewArtSet(&artSet);
    CreateArtSetFromLayer(ai::UnicodeString(SafeguardFile::FOREGROUND_LAYER), artSet);
    
    ProcessArtSet(artSet, [](AIArtHandle handle)
                  {
                      short type;
                      sAIArt->GetArtType(handle, &type);
                      
                      if (type == kTextFrameArt)
                      {
                          int attr = 0;
                          //Check if the art is visible
                          sAIArt->GetArtUserAttr(handle, kArtHidden, &attr);
                          if (!(attr & kArtHidden))
                          {
                              //Check if the art is editable
                              attr = 0;
                              sAIArt->GetArtUserAttr(handle, kArtLocked, &attr);
                              if (attr & kArtLocked)
                              {
                                  sAIArt->SetArtUserAttr(handle, kArtLocked, 0);
                              }
                              
                              //Convert the type to paths
                              AIArtHandle tempNewPaths;
                              sAITextFrame->CreateOutline(handle, &tempNewPaths);
                              sAIArt->DisposeArt(handle);
                          }
                      }
                  });
}
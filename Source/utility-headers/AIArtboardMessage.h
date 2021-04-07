//
//  AIArtboardMessage.h
//  SafeguardTools
//
//  Created by T431962 on 9/30/16.
//
//

#ifndef AIArtboardMessage_h
#define AIArtboardMessage_h

enum Source{
    kSetActive,
    kUpdate,
    kUndoRedo
};

typedef struct
{
    Source msgSrc;
} AIArtboardMessage;


#endif /* AIArtboardMessage_h */

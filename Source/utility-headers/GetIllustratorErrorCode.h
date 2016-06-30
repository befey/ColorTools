//
//  GetIllustratorErrorCode.h
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#ifndef SafeguardTools_GetIllustratorErrorCode_h
#define SafeguardTools_GetIllustratorErrorCode_h

inline string GetIllustratorErrorCode(ASErr err)
{
    char byteArray[5];
    
    // convert from an unsigned long int to a 4-byte array
    byteArray[0] = (int)((err >> 24) & 0xFF) ;
    byteArray[1] = (int)((err >> 16) & 0xFF) ;
    byteArray[2] = (int)((err >> 8) & 0XFF);
    byteArray[3] = (int)((err & 0XFF));
    byteArray[4] = '\0';
    return string(byteArray);
}

#endif
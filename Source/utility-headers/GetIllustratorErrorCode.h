//
//  GetIllustratorErrorCode.h
//  SafeguardTools
//
//  Created by T431962 on 6/23/16.
//
//

#ifndef SafeguardTools_GetIllustratorErrorCode_h
#define SafeguardTools_GetIllustratorErrorCode_h

#include <string>
#include "ASTypes.h"

inline std::string GetIllustratorErrorCode(ASErr err)
{
    char byteArray[5];
    
    // convert from an unsigned long int to a 4-byte array
    byteArray[0] = (char)((err >> 24) & 0xFF);
    byteArray[1] = (char)((err >> 16) & 0xFF);
    byteArray[2] = (char)((err >> 8)  & 0XFF);
    byteArray[3] = (char)((err)       & 0XFF);
    byteArray[4] = '\0';
    return std::string(byteArray);
}

#endif

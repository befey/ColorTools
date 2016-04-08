//
//  VPBFactory.h
//  SafeguardTools
//
//  Created by T431962 on 4/8/16.
//
//

#ifndef __SafeguardTools__VPBFactory__
#define __SafeguardTools__VPBFactory__

#include "VPB.h"

typedef VPB (*SettingsFunction) ();

class VPBFactory
{
public:
    inline VPBFactory(SettingsFunction f) {vpb = f();};
    inline operator VPB(void) const {return vpb;}
private:
    VPB vpb;
    SettingsFunction func;
};

#endif /* defined(__SafeguardTools__VPBFactory__) */

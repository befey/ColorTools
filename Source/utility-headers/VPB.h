/*
 *  VPB.h
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 2/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef VPB_H
#define VPB_H

//=================================
// included dependencies
#include "SDKErrors.h"
#include "AIPDFFormatAction.h"
#include "AIDocumentAction.h"
#include "AIActionManager.h"

//=================================
// forward declared dependencies
extern AIActionManagerSuite* sAIActionManager;

//=================================
// Constant definitions
constexpr auto kSaveACopyAsAction = "adobe_saveACopyAs";

//=================================
// VPB - wrapper for Illustrator action parameters
class VPB
{
public:
    VPB();
    ~VPB();
    VPB(const VPB& src);
    VPB& operator=(const VPB& rhs);
    
    /**	Returns the value parameter block structure.
     */
    //This lets us refer to the private AIActionParamValueRef by using the VPB object itself
    //Casts VPB object to its member
    inline operator AIActionParamValueRef(void) const {return fActionParamValueRef;}
    
private:
    /** The value parameter block.
     */
    AIActionParamValueRef fActionParamValueRef;
    
    void Copy(const AIActionParamValueRef& src);
}; // end class VPB

#endif //VPB_H

/*
 *  VPB.cpp
 *  PrintToPDF
 *
 *  Created by Comp Mac 8a on 2/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "VPB.h"
#include "SDKErrors.h"


VPB::VPB() : fActionParamValueRef(NULL)
{
	ASErr result = kNoErr;
	try {
		SDK_ASSERT(sAIActionManager);
		result = sAIActionManager->AINewActionParamValue(&fActionParamValueRef);
		SDK_ASSERT(!result);
		SDK_ASSERT(fActionParamValueRef);
	}
	catch (ai::Error) {
	}
}

/*
*/
VPB::~VPB()
{
	ASErr result = kNoErr;
	try {
		SDK_ASSERT(sAIActionManager);
		result = sAIActionManager->AIDeleteActionParamValue(fActionParamValueRef);
		SDK_ASSERT(!result);
		fActionParamValueRef = NULL;
	}
	catch (ai::Error) {
	}
}
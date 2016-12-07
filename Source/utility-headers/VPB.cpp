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

VPB::VPB(const VPB& src)
{
    sAIActionManager->AINewActionParamValue(&fActionParamValueRef);
    
    Copy(src.fActionParamValueRef);
}

VPB& VPB::operator=(const VPB& rhs)
{
    if (&rhs != this)
    {
        Copy(rhs.fActionParamValueRef);
    }
    return *this;
}

void VPB::Copy(const AIActionParamValueRef& src)
{
    ai::uint32 count;
    sAIActionManager->AIActionGetValueCount(src, &count);
    for (int i = 0; i < count; i++)
    {
        ActionParamKeyID key;
        sAIActionManager->AIActionGetValueKey(src, i, &key);
        ActionParamTypeID type;
        sAIActionManager->AIActionGetValueType(src, key, &type);
        
        if (type == actionTypeInteger)
        {
            ai::int32 value;
            sAIActionManager->AIActionGetInteger(src, key, &value);
            sAIActionManager->AIActionSetInteger(fActionParamValueRef, key, value);
        }
        else if (type == actionTypeReal)
        {
            AIReal value;
            sAIActionManager->AIActionGetReal(src, key, &value);
            sAIActionManager->AIActionSetReal(fActionParamValueRef, key, value);
        }
        else if (type == actionTypeBoolean)
        {
            AIBoolean value;
            sAIActionManager->AIActionGetBoolean(src, key, &value);
            sAIActionManager->AIActionSetBoolean(fActionParamValueRef, key, value);
        }
        else if (type == actionTypeEnum)
        {
            ai::int32 value;
            ai::UnicodeString name;
            sAIActionManager->AIActionGetEnumeratedUS(src, key, name, &value);
            sAIActionManager->AIActionSetEnumeratedUS(fActionParamValueRef, key, name, value);
        }
        else if (type == actionTypeString)
        {
            ai::UnicodeString value;
            sAIActionManager->AIActionGetStringUS(src, key, value);
            sAIActionManager->AIActionSetStringUS(fActionParamValueRef, key, value);
        }
        else if (type == actionTypeUnitReal)
        {
            ActionParamUnitID unit;
            AIReal value;
            sAIActionManager->AIActionGetUnitReal(src, key, &unit, &value);
            sAIActionManager->AIActionSetUnitReal(fActionParamValueRef, key, unit, value);
        }
    }
}

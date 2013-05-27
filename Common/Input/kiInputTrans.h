#pragma once
#include "systemCommon.h"
#include "commonDefines.h"
#include "Memory/kiMemObject.h"

class kiInputTrans : public kiMemObject
{
	kiInputTrans();
	~kiInputTrans();

	VOID tick();
};
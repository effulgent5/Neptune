#pragma once
#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"

class COMMON_EI kiLock : public kiMemObject
{
public:
	kiLock()
	{
		InitializeCriticalSection(&mCS);
	}

	~kiLock()
	{
		DeleteCriticalSection(&mCS);
	}

	inline void lock()
	{
		EnterCriticalSection(&mCS);
	}

	inline void unlock()
	{
		LeaveCriticalSection(&mCS);
	}
protected:
	CRITICAL_SECTION	mCS;
};
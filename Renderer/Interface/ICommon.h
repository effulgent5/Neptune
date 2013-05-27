#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"

class ICommon
{
public:
	virtual ~ICommon(){}
	virtual UINT KICALL_TYPE free()		{ return 0; }
	virtual UINT KICALL_TYPE obtain()	{ return 0; }
	virtual UINT KICALL_TYPE getRefCount()	{ return 0; }
};
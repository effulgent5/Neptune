#pragma once
#include "IRenderBase.h"

class ISolid : public IRenderBase
{
public:
	virtual ~ISolid(){}

	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE	Dummy_getTypeClass()				= 0;
};

#pragma once
#include "IRenderBase.h"

class IRenderParam : public IRenderBase
{
public:
	virtual ~IRenderParam(){}

	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE	Dummy_getTypeClass() = 0;
};
#pragma once
#include "IRenderParam.h"

class IMetal : public IRenderParam
{
public:
	virtual ~IMetal(){}
	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
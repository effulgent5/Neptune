#pragma once
#include "IRenderBase.h"

class IImage : public IRenderBase
{
public:
	virtual ~IImage(){}
	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
#pragma once
#include "ICommon.h"

class IRenderBase : public ICommon
{
public:
	virtual ~IRenderBase(){}
	virtual BOOL KICALL_TYPE init(VOID* initData, UINT size) = 0;
	virtual VOID KICALL_TYPE finl() = 0;
	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
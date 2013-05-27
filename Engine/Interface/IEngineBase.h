#pragma once
#include "rendererCommon.h"

class IEngineBase : public ICommon
{
public:
	virtual ~IEngineBase(){}
	virtual BOOL KICALL_TYPE init(VOID* initData, UINT size) = 0;
	virtual VOID KICALL_TYPE finl() = 0;
	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
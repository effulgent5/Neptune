#pragma once
#include "IEngineBase.h"

class IStaticMesh : public IEngineBase
{
public:
	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
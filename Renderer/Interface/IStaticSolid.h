#pragma once
#include "ISolid.h"

class IStaticSolid : public ISolid
{
public:
	virtual ~IStaticSolid(){}

	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE	Dummy_getTypeClass()				= 0;
};
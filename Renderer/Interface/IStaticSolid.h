#pragma once
#include "ISolid.h"

class IStaticSolid : public ISolid
{
public:
	virtual ~IStaticSolid(){}

	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE	Dummy_getTypeClass()				= 0;
};
#pragma once
#include "IRenderBase.h"

class ISolid : public IRenderBase
{
public:
	virtual ~ISolid(){}

	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE	Dummy_getTypeClass()				= 0;
};

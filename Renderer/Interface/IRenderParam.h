#pragma once
#include "IRenderBase.h"

class IRenderParam : public IRenderBase
{
public:
	virtual ~IRenderParam(){}

	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE	Dummy_getTypeClass() = 0;
};
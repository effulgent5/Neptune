#pragma once
#include "IRenderParam.h"

class IMetal : public IRenderParam
{
public:
	virtual ~IMetal(){}
	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
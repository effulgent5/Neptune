#pragma once
#include "IRenderBase.h"

class IImage : public IRenderBase
{
public:
	virtual ~IImage(){}
	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
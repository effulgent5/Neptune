#pragma once
#include "ICommon.h"

class IRenderBase : public ICommon
{
public:
	virtual ~IRenderBase(){}
	virtual BOOL KICALL_TYPE init(VOID* initData, UINT size) = 0;
	virtual VOID KICALL_TYPE finl() = 0;
	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
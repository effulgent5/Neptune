#pragma once
#include "IEngineBase.h"

class ISceneItem : public IEngineBase
{
public:
	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
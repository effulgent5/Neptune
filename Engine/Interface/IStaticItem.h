#pragma once
#include "ISceneItem.h"

class IStaticItem : public ISceneItem
{
public:
	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
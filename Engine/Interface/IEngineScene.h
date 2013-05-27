#pragma once
#include "IEngineBase.h"

class ISceneItem;
class IEngineScene : public IEngineBase
{
public:
	virtual ~IEngineScene(){}
	virtual ISceneItem* KICALL_TYPE createItem(const tstring& className, void* initData, UINT size) = 0;
	virtual void KICALL_TYPE appendItem(ISceneItem* item) = 0;

	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
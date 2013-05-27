#pragma once
#include "IEngineBase.h"
#include "IEngineScene.h"

class IEngine : public IEngineBase
{
public:
	virtual ~IEngine() {}
	virtual void KICALL_TYPE tick() = 0;
	virtual IEngineScene* KICALL_TYPE createScene() = 0;
	virtual void KICALL_TYPE setRenderScene(IEngineScene* scene) = 0;

	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
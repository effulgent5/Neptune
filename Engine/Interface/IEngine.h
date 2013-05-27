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

	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
#pragma once
#include "rendererCommon.h"
#include "engineDefines.h"
#include "kiEngineBase.h"

class kiEngineScene;
class IRenderer;
class IRenderScene;
class IEngine;

class kiEngine : public kiEngineBase
{
protected:
	kiEngine();
public:
	virtual ~kiEngine();
	///< IRenderBase Interface
	//virtual UINT KICALL_TYPE	free();
	//virtual UINT KICALL_TYPE	obtain();
	//virtual UINT KICALL_TYPE	getRefCount();
	virtual BOOL KICALL_TYPE	init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE	finl();

	///< IEngine Interface
	virtual void KICALL_TYPE	tick();
	virtual IRenderScene* KICALL_TYPE	createScene();
	virtual void KICALL_TYPE	setRenderScene(IRenderScene* scene);
	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiEngine)

protected:
	kiEngineScene*				mCurScene;
	UINT						mCreateThread;

	static IRenderer*			ms_renderer;
	static kiEngine*			ms_engine;
public:
	static IRenderer*			getInstance();
	static kiEngine*			getEngine();
};
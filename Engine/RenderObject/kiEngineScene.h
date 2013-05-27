#pragma once
#include "rendererCommon.h"
#include "engineDefines.h"
#include "kiEngineBase.h"

class IRenderItem;

class kiEngineScene : public kiEngineBase
{
protected:
	kiEngineScene();
public:	
	virtual ~kiEngineScene();

	///< IRenderBase Interface
	//virtual UINT KICALL_TYPE	free();
	//virtual UINT KICALL_TYPE	obtain();
	//virtual UINT KICALL_TYPE	getRefCount();
	virtual BOOL KICALL_TYPE	init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE	finl();

	///< IRenderScene interface
	virtual IRenderItem* KICALL_TYPE	createItem(const tstring& className, void* initData, UINT size);
	virtual void KICALL_TYPE	appendItem(IRenderItem* item);

	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiEngineScene)
protected:
};
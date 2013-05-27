#pragma once
#include "rendererCommon.h"
#include "engineDefines.h"
#include "kiEngineBase.h"

class kiSceneItem : public kiEngineBase
{
protected:
	kiSceneItem();
public:	
	virtual ~kiSceneItem();

	///< IRenderBase Interface
	virtual BOOL KICALL_TYPE	init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE	finl();

	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiSceneItem)
};
#pragma once
#include "rendererCommon.h"
#include "engineDefines.h"
#include "kiSceneItem.h"

class kiStaticItem : public kiSceneItem
{
protected:
	kiStaticItem();
public:	
	virtual ~kiStaticItem();

	virtual BOOL KICALL_TYPE	init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE	finl();

	IStaticSolid*	getSolid();
	IRenderParam*	getMatParam();

	_DECLARE_DYNAMIC_(kiStaticItem)
protected:
	IStaticSolid*	mSolid;
};
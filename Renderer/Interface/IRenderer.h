#pragma once
#include "IRenderBase.h"
#include "IMetal.h"
#include "IBanal.h"
#include "ICamera.h"
#include "ISolid.h"

class kTransform;

class IRenderer : public IRenderBase
{
public:
	virtual ~IRenderer(){}
	virtual BOOL KICALL_TYPE	resize(int w, int h) = 0;
	virtual INT KICALL_TYPE		begin(ICamera* camera) = 0;
	virtual VOID KICALL_TYPE	renderBanal(ISolid* s, kTransform* t, IBanal* m)	= 0;
	virtual VOID KICALL_TYPE	renderMetal(ISolid* s, kTransform* t, IMetal* m)	= 0;
	virtual INT KICALL_TYPE		end() = 0;
	virtual IRenderBase* KICALL_TYPE	createItem(const tstring& className, void* initData, UINT dataSize) = 0;

	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE	Dummy_getTypeClass()				= 0;
};
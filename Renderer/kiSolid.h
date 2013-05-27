#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderBase.h"

class kiSolid : public kiRenderBase
{
public:
	kiSolid();
	virtual ~kiSolid();

	virtual BOOL KICALL_TYPE init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE finl();

	///< ����������нӿ�֮��
	_DECLARE_DYNAMIC_(kiSolid)
};

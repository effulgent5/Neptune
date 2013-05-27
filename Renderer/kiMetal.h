#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderParam.h"

class kiMetal : public kiRenderParam
{
public:
	kiMetal();
	virtual ~kiMetal();

	virtual BOOL KICALL_TYPE	init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE	finl();

	///< ����������нӿ�֮��
	_DECLARE_DYNAMIC_(kiMetal)
};
#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderBase.h"

class kiMaterial;

class kiRenderParam : public kiRenderBase
{
public:
	kiRenderParam();
	virtual ~kiRenderParam();

	virtual BOOL KICALL_TYPE init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE finl();

	///< ����������нӿ�֮��
	_DECLARE_DYNAMIC_(kiRenderParam)

	inline kiMaterial* getMaterial();

protected:
	kiMaterial*	mMaterial;
};

inline kiMaterial* kiRenderParam::getMaterial()
{
	return mMaterial;
}
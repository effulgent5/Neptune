#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiSolid.h"

class kiStaticSolid : public kiSolid
{
public:
	kiStaticSolid();
	virtual ~kiStaticSolid();

	virtual BOOL KICALL_TYPE init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE finl();

	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiStaticSolid)

	inline ID3D11Buffer* getVB();
	inline ID3D11Buffer* getIB();
protected:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
};

inline ID3D11Buffer* kiStaticSolid::getVB()
{
	return mVB;
}

inline ID3D11Buffer* kiStaticSolid::getIB()
{
	return mIB;
}
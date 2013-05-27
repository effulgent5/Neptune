#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderParam.h"

class IImage;
class kiImage;
class kiBanal : public kiRenderParam
{
public:
	kiBanal();
	virtual ~kiBanal();

	virtual BOOL KICALL_TYPE	init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE	finl();

	virtual IImage* KICALL_TYPE	getBaseTex() const;
	virtual VOID KICALL_TYPE	setBaseTex(IImage* image);
	virtual UINT KICALL_TYPE	getSpcPower() const;
	virtual VOID KICALL_TYPE	setSpcPower(UINT power);
	virtual const kVec3& KICALL_TYPE getColorBias() const;
	virtual VOID KICALL_TYPE	setColorBias(const kVec3& color);

	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiBanal)

protected:
	kVec3						mColorBias;
	UINT						mSpcPower;
	kiImage*					mBaseTex;
};
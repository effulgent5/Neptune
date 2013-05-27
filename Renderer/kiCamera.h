#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderBase.h"

class kiCamera : public kiRenderBase
{
public:
	kiCamera();
	virtual ~kiCamera();

	virtual BOOL KICALL_TYPE init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE finl();

	///< Camera Interface
	virtual const kVec3& KICALL_TYPE getUp();
	virtual const kVec3& KICALL_TYPE getRight();
	virtual const kVec3& KICALL_TYPE getFront();
	virtual const kVec3& KICALL_TYPE getPosition();
	virtual const kFrustum& KICALL_TYPE getFrustum();

	virtual void KICALL_TYPE setUp(const kVec3& val);
	virtual void KICALL_TYPE setRight(const kVec3& val);
	virtual void KICALL_TYPE setFront(const kVec3& val);
	virtual void KICALL_TYPE setPosition(const kVec3& val);
	virtual void KICALL_TYPE setFrustum(const kFrustum& val);

	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiCamera)
protected:

	kVec3		mPosition;
	kVec3		mUp;
	kVec3		mRight;
	kVec3		mFront;
	kFrustum	mFrustum;
};
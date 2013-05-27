#pragma once
#include "IRenderBase.h"

class kVec3;
class kFrustum;

class ICamera : public IRenderBase
{
public:
	virtual ~ICamera(){}

	virtual const kVec3& getUp()				= 0;
	virtual const kVec3& getRight()				= 0;
	virtual const kVec3& getFront()				= 0;
	virtual const kVec3& getPosition()			= 0;
	virtual const kFrustum& getFrustum()		= 0;

	virtual void setUp(const kVec3& val)		= 0;
	virtual void setRight(const kVec3& val)		= 0;
	virtual void setFront(const kVec3& val)		= 0;
	virtual void setPosition(const kVec3& val)	= 0;
	virtual void setFrustum(const kFrustum& val)= 0;

	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE Dummy_getTypeClass() = 0;
};
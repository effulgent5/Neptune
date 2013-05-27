#pragma once
#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"

class kVec3;
class kMatrix33;

class COMMON_EI kQuaternion : public kiMemObject
{
public:
	kQuaternion(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f);
	kQuaternion(float angle, const kVec3& axis);
	kQuaternion(const kQuaternion& quat);
	~kQuaternion();

	void initByAA(float angle, const kVec3& axis);
	void getAA(float& angle, kVec3& axis);

	void initByMatrix33(const kMatrix33& mat);
	void getMatrix33(kMatrix33& mat);

	void normalize();

	float mX, mY, mZ, mW;
};
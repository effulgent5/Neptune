#include "kQuaternion.h"
#include "kVec.h"
#include "kMatrix.h"


kQuaternion::kQuaternion(float w, float x, float y, float z)
: mX(w)
, mY(x)
, mZ(y)
, mW(z)
{

}

kQuaternion::kQuaternion(float angle, const kVec3& axis)
{
	initByAA(angle, axis);
}

kQuaternion::kQuaternion(const kQuaternion& quat)
: mW(quat.mW)
, mX(quat.mX)
, mY(quat.mY)
, mZ(quat.mZ)
{

}


kQuaternion::~kQuaternion()
{

}

void kQuaternion::initByAA(float angle, const kVec3& axis)
{
	float halfAngle = angle*0.5f;
	float sn;
	kSinCos(halfAngle, sn, mW);
	mX = axis.x * sn;
	mY = axis.y * sn;
	mZ = axis.z * sn;
}

void kQuaternion::getAA(float& angle, kVec3& axis)
{
	float len = kSqrt(mX*mX + mY*mY + mZ*mZ);
	if(len < 1e-03f)
	{
		angle = 0.f;
		axis = kVec3::ZERO;
	}
	else
	{
		angle = 2.f*kACos(mW);
		float invLen = 1.f / len;
		axis.x = mX*invLen; axis.y = mY*invLen; axis.z = mZ*invLen;
	}
}

void kQuaternion::initByMatrix33(const kMatrix33& mat)
{
	float trace = mat.getData(0, 0) + mat.getData(1, 1) + mat.getData(2, 2);
}

void kQuaternion::getMatrix33(kMatrix33& mat)
{

}


void kQuaternion::normalize()
{
	float sqrLen = mW*mW + mX*mX + mY*mY + mZ*mZ;
	float invLen = 1.f / kSqrt(sqrLen);
	mW *= invLen; mX *= invLen; mY *= invLen; mZ *= invLen;
}
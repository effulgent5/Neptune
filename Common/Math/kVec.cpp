#include "kVec.h"

const kVec3 kVec3::ZERO(0.f, 0.f, 0.f);
const kVec3 kVec3::UNITX(1.f, 0.f, 0.f);
const kVec3 kVec3::UNITY(0.f, 1.f, 0.f);
const kVec3 kVec3::UNITZ(0.f, 0.f, 1.f);
const kVec3 kVec3::UNITALL(1.f, 1.f, 1.f);

const kVec2 kVec2::ZERO(0.f, 0.f);
const kVec2 kVec2::UNITX(1.f, 0.f);
const kVec2 kVec2::UNITY(0.f, 1.f);
const kVec2 kVec2::UNITALL(1.f, 1.f);

kVec3 operator+(const kVec3& v0, const kVec3& v1)
{
	return kVec3(v0.x+v1.x, v0.y+v1.y, v0.z+v1.z);
}

kVec3 operator-(const kVec3& v0, const kVec3& v1)
{
	return kVec3(v0.x-v1.x, v0.y-v1.y, v0.z-v1.z);
}

kVec3 operator*(const kVec3& v0, const kVec3& v1)
{
	return kVec3(v0.x*v1.x, v0.y*v1.y, v0.z*v1.z);
}

kVec3 operator*(float f, const kVec3& v)
{
	return kVec3(f*v.x, f*v.y, f*v.z);
}

kVec3 operator*(const kVec3& v, float f)
{
	return kVec3(f*v.x, f*v.y, f*v.z);
}

bool operator==(const kVec3& v0, const kVec3& v1)
{
	return (v0.x == v1.x) && (v0.y == v1.y) && (v0.z == v1.z);
}

bool operator!=(const kVec3& v0, const kVec3& v1)
{
	return !(v0 == v1);
}

kVec3 operator/(const kVec3& v0, float f)
{
	float invf = 1.f / f;
	return kVec3(v0.x*invf, v0.y*invf, v0.z*invf);
}

kVec3 cross(const kVec3& v0, const kVec3& v1)
{
	return kVec3(v0.y*v1.z - v0.z*v1.y, v0.z*v1.x - v0.x*v1.z, v0.x*v1.y - v0.y*v1.x);
}

kVec3 normalCross(const kVec3& v0, const kVec3& v1)
{
	kVec3 v(v0.y*v1.z - v0.z*v1.y, v0.z*v1.x - v0.x*v1.z, v0.x*v1.y - v0.y*v1.x);
	float f = v.length();
	if(f < 1e-06f)
		return v / f;
	else
		return kVec3();
}

float dot(const kVec3& v0, const kVec3& v1)
{
	return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

///***********************************************************
kVec2 operator+(const kVec2& v0, const kVec2& v1)
{
	return kVec2(v0.x+v1.x, v0.y+v1.y);
}

kVec2 operator-(const kVec2& v0, const kVec2& v1)
{
	return kVec2(v0.x-v1.x, v0.y-v1.y);
}

kVec2 operator*(const kVec2& v0, const kVec2& v1)
{
	return kVec2(v0.x*v1.x, v0.y*v1.y);
}
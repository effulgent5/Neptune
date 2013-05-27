#pragma once
#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"
#include "kMath.h"

class COMMON_EI kVec2 : public kiMemObject
{
public:
	kVec2(float x_=0.f, float y_=0.f)
		: x(x_), y(y_)
	{

	}

	float length() const
	{
		return kSqrt(x*x + y*y);
	}

	float sqrLength() const
	{
		return x*x + y*y;
	}

	float& operator[](int i)
	{
		float* base = &x;
		return (float&)base[i];
	}

	kVec2& operator+=(const kVec2& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	kVec2& operator-=(const kVec2& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	kVec2 operator-() const
	{
		return kVec2(-x, -y);
	}

	float x, y;

	static const kVec2 ZERO;
	static const kVec2 UNITX;
	static const kVec2 UNITY;
	static const kVec2 UNITALL;
};

class COMMON_EI kVec3 : public kiMemObject
{
public:
	kVec3(float x_=0.f, float y_=0.f, float z_=0.f)
		: x(x_), y(y_), z(z_)
	{

	}

	kVec3(const kVec3& vec)
		:x(vec.x), y(vec.y), z(vec.z)
	{

	}

	float length() const
	{
		return kSqrt(x*x + y*y + z*z);
	}

	float sqrLength() const
	{
		return x*x + y*y + z*z;
	}

	float normalize()
	{
		float f = length();
		if(f > 1e-06f)
		{
			float recip = 1.0f / f;
			x *= recip;		y *= recip;		z *= recip;
		}
		else
		{
			x = y = z = 0.f;
			f = 0.f;
		}

		return f;
	}

	float& operator[](int i)
	{
		float* base = &x;
		return (float&)base[i];
	}

	kVec3& operator+=(const kVec3& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	kVec3& operator-=(const kVec3& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	kVec3 operator-() const
	{
		return kVec3(-x, -y, -z);
	}

	float x, y, z;

	static const kVec3 ZERO;
	static const kVec3 UNITX;
	static const kVec3 UNITY;
	static const kVec3 UNITZ;
	static const kVec3 UNITALL;
};

extern COMMON_EI kVec3 operator+(const kVec3& v0, const kVec3& v1);
extern COMMON_EI kVec3 operator-(const kVec3& v0, const kVec3& v1);
extern COMMON_EI kVec3 operator*(const kVec3& v0, const kVec3& v1);
extern COMMON_EI kVec3 operator*(float f, const kVec3& v);
extern COMMON_EI kVec3 operator*(const kVec3& v, float f);
extern COMMON_EI bool operator==(const kVec3& v0, const kVec3& v1);
extern COMMON_EI bool operator!=(const kVec3& v0, const kVec3& v1);
extern COMMON_EI kVec3 operator/(const kVec3& v0, float f);
extern COMMON_EI kVec3 cross(const kVec3& v0, const kVec3& v1);
extern COMMON_EI kVec3 normalCross(const kVec3& v0, const kVec3& v1);
extern COMMON_EI float dot(const kVec3& v0, const kVec3& v1);

extern COMMON_EI kVec2 operator+(const kVec2& v0, const kVec2& v1);
extern COMMON_EI kVec2 operator-(const kVec2& v0, const kVec2& v1);
extern COMMON_EI kVec2 operator*(const kVec2& v0, const kVec2& v1);
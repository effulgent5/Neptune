#pragma once
#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"
#include "kMath.h"
#include "kVec.h"

class kAABB;
class kRay;

class COMMON_EI kPlane : public kiMemObject
{
public:
	enum{NO_SIDE, PSIDE, NSIDE};
	kPlane()
	{
		mNormal = kVec3::ZERO;
		mDist	= 0.f;
	}

	kPlane(const kVec3& normal, float dist)
	{
		mNormal = normal;
		mDist	= dist;
	}

	kPlane(const kVec3& normal, const kVec3& point)
	{
		mNormal = normal;
		mDist	= dot(normal, point);
	}

	kPlane(const kVec3& p0, const kVec3& p1, const kVec3& p2)
	{
		kVec3 diff0 = p1 - p0;
		kVec3 diff1 = p2 - p1;
		mNormal = normalCross(diff1, diff0);
		mDist	= dot(mNormal, p0);
	}

	bool operator==(const kPlane& plane){ return (mNormal == plane.mNormal) && (mDist == plane.mDist); }
	bool operator!=(const kPlane& plane){ return !(*this == plane); }

	void setNormal(const kVec3& normal)
	{
		mNormal = normal;
	}

	const kVec3& getNormal()
	{
		return mNormal;
	}

	void setDist(float dist)	{ mDist = dist; }
	float getDist()				{ return mDist; }

	bool collideRay(const kRay& ray, bool& pside, float& distToRayOrigin) const;

	float distance(const kVec3& point){ return dot(mNormal, point) - mDist; }

	int whichSide(const kVec3& point)
	{
		float dist = dot(mNormal, point) - mDist;
		if(dist < 0.f)
			return kPlane::NSIDE;
		else if(dist > 0.f)
			return kPlane::PSIDE;
		else
			return kPlane::NO_SIDE;
	}

private:
	kVec3	mNormal;
	float	mDist;
};

///<******************************************************
class COMMON_EI kRay : public kiMemObject
{
public:
	kRay(const kVec3& o, const kVec3& d);
	~kRay();

	bool IntersectTri(const kVec3& kV1, const kVec3& kV2, const kVec3& kV3,\
						bool bCull, kVec3& kIntersect, float& r, float& s, float& t) const;

	bool IntersectAABB(const kAABB& box, bool& inside, float& intersectDist) const;
	bool IntersectAABB(const kAABB& box) const;

	kVec3	mOrigin;
	kVec3	mDirection;
};
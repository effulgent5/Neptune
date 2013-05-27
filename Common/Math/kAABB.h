#pragma once

#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"
#include "kMath.h"
#include "kVec.h"

class kPlane;
class kRay;

class COMMON_EI kAABB : public kiMemObject
{
public:
	kAABB();
	//kAABB( const sdHeightMap *pkHeightMap );
	~kAABB();

	struct stValueIndices
	{
		int i0, i1, i2;
	};

	enum eVexIndex
	{
		BLD_VEX		= 0,
		BLU_VEX		= 1,
		BRU_VEX		= 2,
		BRD_VEX		= 3,
		TLD_VEX		= 4,
		TLU_VEX		= 5,
		TRU_VEX		= 6,
		TRD_VEX		= 7
	};

	enum ePlaneIndex
	{
		WEST_PLANE	= 0,
		EAST_PLANE	= 1,
		SOUTH_PLANE	= 2,
		NORTH_PLANE	= 3,
		BOTTOM_PLANE = 4,
		TOP_PLANE	= 5
	};

	void				setMinVert( const kVec3& kMinPt );
	void				setMaxVert( const kVec3& kMaxPt );
	kVec3				getMinVert() const;
	kVec3				getMaxVert() const;
	kVec3				getCenter() const;
	float				Distance2Point(kVec3 &kPoint);
	////判断是否与圆相交
	//bool				CollisionWithCircle2D( const NiPoint2 &kOrigin, float fRadius );
	//假设所有平面的法线都是指向椎体内部的
	bool				CollisionWithFrustum(kPlane* planes, unsigned int size);
	bool				CollisionWithPlane(kPlane& plane);
	bool				IsPointInBox(const kVec3& kPt);
	//bool				IsPointInBox2D(const kVec3& kPt);
	//取得边长
	float				getXExtend() const;
	float				getYExtend() const;
	float				getZExtend() const;
	float&				getMinZ();
	float&				getMaxZ();
	void					setMinZ(float f);
	void					setMaxZ(float f);

	void				MergePoint( const kVec3& kPoint );
	kVec3				getVertex(eVexIndex index);
	kPlane				getPlane(ePlaneIndex index) const;
protected:
	static void			InitAABBTable();
	UINT				GetNormalCode(const kVec3& kNormal);
private:
	static UINT					VertIndex[8];
	static stValueIndices		ValueIndex[8];
	float						Verts[6];		//minX, minY, minZ, maxX, maxY, maxZ
};

inline void kAABB::setMinZ(float f)
{
	Verts[2] = f;
}

inline void kAABB::setMaxZ(float f)
{
	Verts[5] = f;
}

inline float& kAABB::getMinZ()
{
	return Verts[2];
}

inline float& kAABB::getMaxZ()
{
	return Verts[5];
}

inline void kAABB::setMinVert( const kVec3& kMinPt )
{
	Verts[0] = kMinPt.x; Verts[1] = kMinPt.y; Verts[2] = kMinPt.z;
}

inline void kAABB::setMaxVert( const kVec3& kMaxPt )
{
	Verts[3] = kMaxPt.x; Verts[4] = kMaxPt.y; Verts[5] = kMaxPt.z;
}

inline kVec3 kAABB::getMinVert() const
{
	return kVec3( Verts[0], Verts[1], Verts[2] );
}

inline kVec3 kAABB::getMaxVert() const
{
	return kVec3( Verts[3], Verts[4], Verts[5] );
}

inline kVec3 kAABB::getCenter() const
{
	return kVec3((Verts[0] + Verts[3])/2.f, (Verts[1] + Verts[4])/2.f, (Verts[2] + Verts[5])/2.f);
}

inline float kAABB::getXExtend() const
{
	return Verts[3] - Verts[0];
}

inline float kAABB::getYExtend() const
{
	return Verts[4] - Verts[1];
}

inline float kAABB::getZExtend() const
{
	return Verts[5] - Verts[2];
}
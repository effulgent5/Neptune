#include "kAABB.h"
#include "kGeom.h"

kAABB::stValueIndices	kAABB::ValueIndex[8];
UINT					kAABB::VertIndex[8];

kAABB::kAABB()
{
	InitAABBTable();
	memset(Verts, 0, 6*sizeof(float));
}


kAABB::~kAABB()
{

}

void kAABB::InitAABBTable()
{
	static bool bInited = false;
	if ( !bInited )
	{
		//测试顶点查找表
		VertIndex[0] = 1;
		VertIndex[1] = 4;
		VertIndex[2] = 7;
		VertIndex[3] = 2;
		VertIndex[4] = 3;
		VertIndex[5] = 6;
		VertIndex[6] = 5;
		VertIndex[7] = 0;

		//值索引表
		ValueIndex[0].i0 = 0; ValueIndex[0].i1 = 1; ValueIndex[0].i2 = 2;
		ValueIndex[1].i0 = 3; ValueIndex[1].i1 = 4; ValueIndex[1].i2 = 5;
		ValueIndex[2].i0 = 3; ValueIndex[2].i1 = 1; ValueIndex[2].i2 = 2;
		ValueIndex[3].i0 = 0; ValueIndex[3].i1 = 4; ValueIndex[3].i2 = 5;
		ValueIndex[4].i0 = 3; ValueIndex[4].i1 = 4; ValueIndex[4].i2 = 2;
		ValueIndex[5].i0 = 0; ValueIndex[5].i1 = 1; ValueIndex[5].i2 = 5;
		ValueIndex[6].i0 = 0; ValueIndex[6].i1 = 4; ValueIndex[6].i2 = 2;
		ValueIndex[7].i0 = 3; ValueIndex[7].i1 = 1; ValueIndex[7].i2 = 5;
		
		bInited = true;
	}
}


bool kAABB::CollisionWithFrustum(kPlane* planes, unsigned int size)
{
	for( unsigned int ui = 0; ui < size; ++ui )
	{
		kPlane& rPlane = planes[ui];

        /// 根据当前面的法线，找到最有可能在面以内的Bounding Box顶点的序号。
        /// 注意：Frustum的Plane的法线朝内。
		UINT vertIndex = VertIndex[GetNormalCode(rPlane.getNormal())];

        /// 根据顶点序号，找到该顶点的xyz值分别对应minX, minY, minZ, maxX, maxY, maxZ中的哪一个值。
        stValueIndices& xyzIndex = ValueIndex[vertIndex];

        /// 得到顶点位置。
        kVec3 vertPos(Verts[xyzIndex.i0], Verts[xyzIndex.i1], Verts[xyzIndex.i2]);

        /// 得到顶点与面的距离。
		float fDistance = dot(rPlane.getNormal(), vertPos) - rPlane.getDist();

        /// 如果该顶点在plane之外，说明其他顶点也在plane之外，整个AABB Bounding Box与frustum无法相交。
		if ( fDistance < 0.0f )
			return false;
	}

	return true;
}

bool kAABB::CollisionWithPlane(kPlane& plane)
{
	kVec3 kNormal = plane.getNormal();
	UINT dwCode = GetNormalCode(kNormal);

	//是否完全位于背面
	UINT dwVertIndex = VertIndex[dwCode];
	float fDistance = \
		kNormal.x * Verts[ ValueIndex[dwVertIndex].i0 ] + \
		kNormal.y * Verts[ ValueIndex[dwVertIndex].i1 ] + \
		kNormal.z * Verts[ ValueIndex[dwVertIndex].i2 ] - plane.getDist();

	if ( fDistance < 0.0f )
		return false;

	//是否完全位于正面
	dwCode = (~dwCode) & 0x00000007;
	dwVertIndex = VertIndex[dwCode];
	fDistance = \
		kNormal.x * Verts[ ValueIndex[dwVertIndex].i0 ] + \
		kNormal.y * Verts[ ValueIndex[dwVertIndex].i1 ] + \
		kNormal.z * Verts[ ValueIndex[dwVertIndex].i2 ] - plane.getDist();

	if ( fDistance > 0.0f )
		return false;

	return true;
}

bool kAABB::IsPointInBox( const kVec3& kPt )
{
	if( kPt.x > Verts[3] || kPt.x < Verts[0] || \
		kPt.y > Verts[4] || kPt.y < Verts[1] || \
		kPt.z > Verts[5] || kPt.z < Verts[2] )
	{
		return false;
	}
	return true;
}

float kAABB::Distance2Point(kVec3 &kPoint)
{
	kVec3 kDistance;

	for (int i = 0; i < 3; i++)
	{
		kDistance[i] = kMin(
							kMin(kPoint[i]-Verts[i], Verts[3+i]-kPoint[i]),
							0.0f );
	}

	return kDistance.sqrLength();
}

void kAABB::MergePoint( const kVec3& kPoint )
{
	if( kPoint.x < Verts[0] ) Verts[0] = kPoint.x;
	if( kPoint.y < Verts[1] ) Verts[1] = kPoint.y;
	if( kPoint.z < Verts[2] ) Verts[2] = kPoint.z;

	if( kPoint.x > Verts[3] ) Verts[3] = kPoint.x;
	if( kPoint.y > Verts[4] ) Verts[4] = kPoint.y;
	if( kPoint.z > Verts[5] ) Verts[5] = kPoint.z;
}

UINT kAABB::GetNormalCode(const kVec3 &kNormal )
{
	const unsigned int* px = reinterpret_cast<const unsigned int*>(&kNormal.x);
	const unsigned int* py = reinterpret_cast<const unsigned int*>(&kNormal.y);
	const unsigned int* pz = reinterpret_cast<const unsigned int*>(&kNormal.z);

    

    //return sign(*px) << 2     |      sign(*py) << 1      |       sign(*pz)    ;
    //因为Bit 31是IEEE32浮点数的符号位，所以等价于：
    //    ((*px) >> 31 & 0x1) << 2 | ((*py) >> 31 & 0x1) << 1 | ((*pz) >> 31 & 0x1);
    //==>    ((*px) >> 29 & 0x4)   |    ((*py) >> 30 & 0x2)   | ((*pz) >> 31 & 0x1);

	return ((*px) >> 29 & 0x4) | ((*py) >> 30 & 0x2) | ((*pz) >> 31 & 0x1);
}

kVec3 kAABB::getVertex(kAABB::eVexIndex index)
{
	///< from min vertex clockwise, bottom first
	switch(index)
	{
		///< bottom four vertex
	case BLD_VEX:
		return kVec3(Verts[0], Verts[1], Verts[2]);
		break;
	case BLU_VEX:
		return kVec3(Verts[0], Verts[4], Verts[2]);
		break;
	case BRU_VEX:
		return kVec3(Verts[3], Verts[4], Verts[2]);
		break;
	case BRD_VEX:
		return kVec3(Verts[3], Verts[1], Verts[2]);
		break;
		///< top four vertex
	case TLD_VEX:
		return kVec3(Verts[0], Verts[1], Verts[5]);
		break;
	case TLU_VEX:
		return kVec3(Verts[0], Verts[4], Verts[5]);
		break;
	case TRU_VEX:
		return kVec3(Verts[3], Verts[4], Verts[5]);
		break;
	case TRD_VEX:
		return kVec3(Verts[3], Verts[1], Verts[5]);
		break;
	default:
		return kVec3::ZERO;
		break;
	}
}

kPlane kAABB::getPlane(kAABB::ePlaneIndex index) const
{
	///< from min vertex clockwise, bottom first
	kVec3 maxV = getMaxVert();
	kVec3 minV = getMinVert();
	switch(index)
	{
	case WEST_PLANE:
		return kPlane(-kVec3::UNITX, minV);
		break;
	case EAST_PLANE:
		return kPlane(kVec3::UNITX, maxV);
		break;
	case SOUTH_PLANE:
		return kPlane(-kVec3::UNITY, minV);
		break;
	case NORTH_PLANE:
		return kPlane(kVec3::UNITY, maxV);
		break;
	case BOTTOM_PLANE:
		return kPlane(-kVec3::UNITZ, minV);
		break;
	case TOP_PLANE:
		return kPlane(kVec3::UNITZ, maxV);
		break;
	default:
		return kPlane();
		break;
	}
}
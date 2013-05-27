#include "kGeom.h"
#include "kAABB.h"

bool kPlane::collideRay(const kRay& ray, bool& pside, float& distToRayOrigin) const
{
	float distOriginToPlane = dot(mNormal, ray.mOrigin);
	float dist = distOriginToPlane - mDist;
	if(dist <= 0.f)
		pside = true;
	else
		pside = false;

	float cosValue = dot(mNormal, ray.mDirection);
	if(cosValue == 0)	///< parallel
	{
		distToRayOrigin = -1.f;
		return false;
	}

	distToRayOrigin = (mDist - distOriginToPlane) / cosValue;
	return true;
}

///<***************************************************************
kRay::kRay(const kVec3& o, const kVec3& d)
	: mOrigin(o)
	, mDirection(d)
{

}

kRay::~kRay()
{

}

bool kRay::IntersectTri(const kVec3& kV1, const kVec3& kV2, const kVec3& kV3,\
						bool bCull, kVec3& kIntersect, float& r, float& s, float& t) const
{
	// All input quantities are in model space of the NiMesh object.
	// Input:
	//     ray (kOrigin+T*kDir), 
	//     triangle vertices (kV1, kV2, kV3)
	//     backface culling is performed when bCull=true
	// Return value:  true iff the ray intersects the triangle
	// Output (valid when return value is true):
	//     intersection of ray and triangle (kIntersect)
	//     kIntersect = kOrigin+r*kDir
	//     kIntersect = kV1+s*(kV2-kV1)+t*(kV3-kV1)

	// From "Real-Time Rendering" which references Moller, Tomas & Trumbore, 
	// "Fast, Minimum Storage Ray-Triangle Intersection", Journal of Graphics 
	// Tools, vol. 2, no. 1, pp 21-28, 1997. With some modifications to the 
	// non-culling case by Michael Mounier.
	const float fTolerance = 1e-05f;

	kVec3 kEdge1 = kV3 - kV1;
	kVec3 kEdge2 = kV2 - kV1;

	kVec3 kPt = cross(mDirection, kEdge2);

	float fDet = dot(kEdge1, kPt);
	if (fDet >= fTolerance) // Determinant is positive.
	{
		kVec3 kS = mOrigin - kV1;
		s = dot(kS, kPt);

		if (s < 0.0f || s > fDet)
			return false;

		kVec3 q = cross(kS ,kEdge1);
		t = dot(mDirection, q);

		if (t < 0.0f || s + t > fDet)
			return false;

		r = dot(kEdge2, q);

		if (r < 0.0f)
			return false;
	}
	else if (fDet <= -fTolerance && !bCull) // Determinant is negative.
	{
		kVec3 kS = mOrigin - kV1;
		s = dot(kS, kPt);

		if (s > 0.0f || s < fDet)
			return false;

		kVec3 q = cross(kS ,kEdge1);
		t = dot(mDirection, q);

		if (t > 0.0f || s + t < fDet)
			return false;

		r = dot(kEdge2, q);

		if (r > 0.0f)
			return false;
	}
	else    // Parallel ray or culled.
	{
		return false;
	}

	float inv_det = 1.0f / fDet;

	s *= inv_det;
	t *= inv_det;
	r *= inv_det;

	kIntersect = mOrigin + r * mDirection;

	return true;
}

bool kRay::IntersectAABB(const kAABB& box) const
{
	bool inside = true;

	kVec3 origin = mOrigin;
	kVec3 dir = mDirection;

	kVec3 maxT(-1, -1, -1);
	kVec3 boxMinVex = box.getMinVert();
	kVec3 boxMaxVex = box.getMaxVert();

	int i = 0;
	for(i=0; i<3; i++ )
	{
		if( origin[i] < boxMinVex[i] )
		{
			inside = false;
			if( dir[i] > 0 )
			{
				maxT[i] = (boxMinVex[i] - origin[i])/ dir[i];
			}
		}
		else if( origin[i] > boxMaxVex[i] )
		{
			inside = false;
			if( dir[i] < 0 )
			{
				maxT[i] = (boxMaxVex[i] - origin[i]) / dir[i];
			}
		}
	}

	if(inside)
		return true;

	int whichPlane = 0;
	if( maxT[1] > maxT[whichPlane])
		whichPlane = 1;
	if( maxT[2] > maxT[whichPlane])
		whichPlane = 2;

	if( ((int)maxT[whichPlane]) & 0x80000000 )
		return false;

	for(i=0; i<3; i++ )
	{
		if( i!= whichPlane )
		{
			float f = origin[i] + maxT[whichPlane] * dir[i];
			if ( f < (boxMinVex[i] - 0.00001f) ||
				f > (boxMaxVex[i] +0.00001f ) )
			{
				return false;
			}
		}
	}

	return true;
}

bool kRay::IntersectAABB(const kAABB& box, bool& inside, float& intersectDist) const
{
	kVec3 kRayNormalized = mDirection;

	kPlane akPlanes[6];
	//kVec3 akPlaneIntersects[6];

	akPlanes[0] = box.getPlane(kAABB::EAST_PLANE);
	akPlanes[1] = box.getPlane(kAABB::NORTH_PLANE);
	akPlanes[2] = box.getPlane(kAABB::TOP_PLANE);
	akPlanes[3] = box.getPlane(kAABB::WEST_PLANE);
	akPlanes[4] = box.getPlane(kAABB::SOUTH_PLANE);
	akPlanes[5] = box.getPlane(kAABB::BOTTOM_PLANE);

	float fIn = 0;
	float fOut = 0;
	int iInPlane = 0;
	int iInsideCount = 0;
	bool bValidIn = false;
	bool bValidOut = false;

	for (unsigned int uiIndex = 0; uiIndex < 6; uiIndex ++)
	{
		bool pside = false;
		float dist = 0.f;
		bool bIntersect = akPlanes[uiIndex].collideRay(*this, pside, dist);
		//akPlaneIntersects[uiIndex] = mOrigin + dist*mDirection;

		if (pside)
			iInsideCount ++;

		if (bIntersect)
		{
			// need to determine if this is an "in" intersection or an "out"
			// intersection
			bool bIsIn = !pside;

			if (bIsIn && (!bValidIn || (bValidIn && dist > fIn)))
			{
				fIn = dist;
				bValidIn = true;
				iInPlane = uiIndex;
			}
			else if (dist >= 0.0f && !bIsIn && (!bValidOut || (bValidOut && dist < fOut)))
			{
				fOut = dist;
				bValidOut = true;
			}
		}
		else if (!pside)
		{
			intersectDist = -1.f;
			return false;
		}
	}

	// ray begins inside box.
	if (iInsideCount == 6)
	{
		inside = true;
		intersectDist = 0.f;

		return true;
	}

	if (!bValidIn || !bValidOut || fIn > fOut)
	{
		intersectDist = -1.f;
		return false;
	}

	//kVec3 pt = akPlaneIntersects[iInPlane];
	inside = false;
	intersectDist = fIn;

	return true;
}
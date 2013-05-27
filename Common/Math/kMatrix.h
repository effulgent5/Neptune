#pragma once
#include "Memory/kiMemObject.h"
#include "systemCommon.h"
#include "commonDefines.h"
#include "kVec.h"

class COMMON_EI kMatrix33 : public kiMemObject
{
public:
	kMatrix33();
	~kMatrix33();

	float getData(unsigned int row, unsigned int col) const
	{
		return mData[row][col];
	}

	void setData(unsigned int row, unsigned int col, float data)
	{
		mData[row][col] = data;
	}

	kVec3 getRow(unsigned int row) const
	{
		return kVec3(mData[row][0], mData[row][1], mData[row][2]);
	}

	kVec3 getCol(unsigned int col) const
	{
		return kVec3(mData[0][col], mData[1][col], mData[2][col]);
	}

	void setRow(unsigned int row, const kVec3& data)
	{
		mData[row][0] = data.x; mData[row][1] = data.y; mData[row][2] = data.z;
	}

	void setCol(unsigned int col, const kVec3& data)
	{
		mData[0][col] = data.x; mData[1][col] = data.y; mData[2][col] = data.z;
	}

	float mData[3][3];
};


///< **************************************************
class kMatrix34 : public kiMemObject
{

};

///< **************************************************
class kMatrix44 : public kiMemObject
{

};
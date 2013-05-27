#pragma once

struct finalVex
{
	unsigned short pos[4];	/// pos.xyz 
	unsigned short nor[4];	/// normal uv0.x
	unsigned short tgt[4];	/// tangent uv0.y
	//unsigned int   clr0;
	unsigned int   bi;	/// 3 bone 10bit 10bit 10bit  2bit n/a
	unsigned int   bw;	/// 10bit 10bit weight	12bit n/a
};

#define OBJ_NAME_LENGTH 16

struct secDesc
{
	char	name[OBJ_NAME_LENGTH];	///< mesh name
	UINT	flag;		///< 0bit(static or skin)
	UINT	v_start;
	UINT	v_size;
	UINT	i_start;
	UINT	i_size;
};

struct meshHeader
{
	UINT	flag;	///< nevi
	UINT	version;	///< 0x00000001
	UINT	secNum;
};

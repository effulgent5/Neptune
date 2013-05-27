#pragma once
#include "commonExporter.h"

class ssExporter;

struct kiPoint3
{
	float x, y, z;
};

struct vexRef
{
	int		posRef;
	int		norRef;
	int		tbRef;
	int		color0Ref;
	int		alphaRef;
	int		tex0Ref;
	int		tex1Ref;
	int		boneIndex[4];
	float	boneWeight[4];
};

struct finalVex
{
	unsigned short pos[4];	/// pos.xyz 
	unsigned short nor[4];	/// normal uv0.x
	unsigned short tgt[4];	/// tangent uv0.y
	//unsigned int   clr0;
	unsigned int   bi;	/// 3 bone 10bit 10bit 10bit  2bit n/a
	unsigned int   bw;	/// 10bit 10bit weight	12bit n/a
};

class kiMeshUtility
{
public:
	static BOOL exportGeo(TCHAR* path, TCHAR* fileName, ssExporter* exporter);
	static BOOL exportIGameNode_r(IGameNode* node);

protected:
	static BOOL inspectMeshNode(IGameNode* meshNode);
	static BOOL dumpMesh(vector<finalVex>& vexs, vector<unsigned short>& indices, IGameMesh* iGameMesh, BOOL& skined, const char* name);
	static void compactVex(vector<vexRef>& orgin, vector<vexRef>& compact, vector<unsigned short>& index);
	static unsigned short float32Tofloat16(float f);
};
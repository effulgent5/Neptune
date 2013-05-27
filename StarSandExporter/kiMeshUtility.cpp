#include "kiMeshUtility.h"
#include "ssExporter.h"
#include "kiMeshStream.h"

BOOL kiMeshUtility::exportGeo(TCHAR* path, TCHAR* fileName, ssExporter* exporter)
{
	//tchrlen
	//TCHAR* lastSlash = tchrrfind(path, TEXT('\\'));
	//if(lastSlash != NULL)
	//{
	//}
	//size_t size = strlen(path);
	//char last = *(path+size-1);
	//if(last != '\\' && last != '/')
	//{
	//	*(path+size) = '\\';
	//	*(path+size+1) = 0;
	//}

	//strcat_s(path, MAX_PATH, fileName);
	//strcat_s(path, MAX_PATH, ".vrt");
	IGameScene* iGameScene = GetIGameInterface();	
	iGameScene->InitialiseIGame();
	MCHAR* sceneFileName = iGameScene->GetSceneFileName();
	///< 检查欲导出网格的合法性
	Tab<IGameNode*> meshNodes = iGameScene->GetIGameNodeByType(IGameObject::IGAME_MESH);
	int meshNodeNum = meshNodes.Count();
	int legalMeshNum = 0;
	for (int i = 0; i < meshNodeNum; ++i)
	{
		BOOL result = inspectMeshNode(meshNodes[i]);
		if (!result)
			meshNodes[i] = NULL;
		else
			++legalMeshNum;
	}

	kiMeshStream ms;
	if(legalMeshNum > 0)
	{
		TCHAR* fileName = TEXT("d:\\first.mesh");
		BOOL result = ms.open(fileName);
		if(!result)
		{
			::MessageBox(NULL, fileName, TEXT("无法打开指定文件"), MB_ICONERROR);
			return FALSE;
		}
	}

	struct meshHeader meshFileHeader;
	meshFileHeader.flag		= 'iven';
	meshFileHeader.version	= 0x01;
	meshFileHeader.secNum	= (UINT)legalMeshNum;
	ms.write((void*)&meshFileHeader, sizeof(meshFileHeader));

	UINT secTableSize = meshFileHeader.secNum * sizeof(secDesc);
	secDesc* secs = (secDesc*)malloc(secTableSize);
	memset(secs, 0, secTableSize);
	UINT secTablePt = ms.getPointer();
	ms.write((void*)secs, secTableSize);
	UINT viDataPt = ms.getPointer();

	vector<finalVex> vexs;
	vector<unsigned short> indices;
	///< 导出mesh
	UINT totalVIData = 0;
	for (int i = 0; i < meshNodeNum; ++i)
	{
		IGameNode* iGameNode = meshNodes[i];
		if(iGameNode != NULL)
		{			
			IGameMesh* iGameMesh = (IGameMesh*)iGameNode->GetIGameObject();
			TSTR name = iGameNode->GetName();
			iGameMesh->SetCreateOptimizedNormalList();
			if(iGameMesh->InitializeData())
			{
				vexs.clear();
				indices.clear();
				BOOL skined = FALSE;
				BOOL result = dumpMesh(vexs, indices, iGameMesh, skined, name.data());
				if(!result)
				{
					ms.deleteFile();
					free(secs);
					return FALSE;
				}

				UINT vexCount = (UINT)vexs.size();
				UINT idxCount = (UINT)indices.size();
				if(vexCount != 0 && idxCount != 0)
				{
					ms.write(&(vexs[0]), vexCount * sizeof(finalVex));
					ms.write(&(indices[0]), idxCount * sizeof(unsigned short));
				}

				///< 填写节表
				UINT nameLength = name.Length();
				memcpy_s(secs[i].name, OBJ_NAME_LENGTH, name.data(), nameLength);
				secs[i].flag		= (sizeof(finalVex) << 16) | (UINT)skined;
				secs[i].v_start		= (i == 0) ? viDataPt : (secs[i-1].v_start + secs[i-1].v_size + secs[i-1].i_size);
				secs[i].v_size		= sizeof(finalVex) * vexCount;
				secs[i].i_start		= secs[i].v_start + secs[i].v_size;
				secs[i].i_size		= sizeof(unsigned short) * idxCount;

				totalVIData += (secs[i].v_size + secs[i].i_size);
			}
			iGameNode->ReleaseIGameObject();
		}
		///<
	}

	///< 重新写入节表
	ms.movePointer(secTablePt);
	ms.write((void*)secs, secTableSize);
	
	///< 校验文件尺寸
	UINT fileSize = ms.getSize();
	UINT byteSize = sizeof(meshHeader) + secTableSize + totalVIData;
	if(fileSize != byteSize)
	{
		ms.deleteFile();
		::MessageBox(NULL, fileName, TEXT("数据写入出错"), MB_ICONERROR);
		return FALSE;
	}
	//int topObjectNum = iGameScene->GetTopLevelNodeCount();
	//for(int loop = 0; loop < topObjectNum; loop++)
	//{
	//	IGameNode* iGameNode = iGameScene->GetTopLevelNode(loop);
	//	iGameNode->IsGroupOwner()
	//	if(iGameNode->IsTarget() || iGameNode->IsNodeHidden())
	//		continue;

	//	BOOL result = exportIGameNode_r(iGameNode);
	//	if(!result)
	//		return FALSE;
	//}
	ms.close();
	free(secs);
	return TRUE;
}

BOOL kiMeshUtility::inspectMeshNode(IGameNode* meshNode)
{
	IGameObject* obj = meshNode->GetIGameObject();
	IGameMaterial* objMat = meshNode->GetNodeMaterial();
	TSTR name = meshNode->GetName();

	if(name.Length() >= OBJ_NAME_LENGTH)
	{
		::MessageBox(NULL, TEXT("网格对象名字超过15个字符."), name.data(), MB_ICONERROR);
		return FALSE;
	}

	if(meshNode->IsGroupOwner() || meshNode->IsNodeHidden())
	{
		::MessageBox(NULL, TEXT("网格对象为隐藏属性."), name.data(), MB_ICONERROR);
		return FALSE;
	}

	IGameObject::MaxType type = obj->GetMaxType();
	if(objMat != NULL)
	{
		MCHAR* s0 = objMat->GetMaterialClass();
		MCHAR* s1 = objMat->GetMaterialName();
		if(strcmp(s0, "Standard") != 0)
		{
			::MessageBox(NULL, TEXT("网格对象的材质不是standard标准材质."), name.data(), MB_ICONERROR);
			return FALSE;
		}
	}

	if(obj->GetIGameType() != IGameObject::IGAME_MESH)
	{
		::MessageBox(NULL, TEXT("欲导出对象不是网格对象."), name.data(), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

BOOL kiMeshUtility::exportIGameNode_r(IGameNode* node)
{
	//IGameObject* obj = node->GetIGameObject();
	//IGameMaterial* objMat = node->GetNodeMaterial();
	//TSTR name = node->GetName();

	//if(!node->IsGroupOwner() && !node->IsNodeHidden())
	//{
	//	switch(obj->GetIGameType())
	//	{
	//	case IGameObject::IGAME_MESH:
	//		{
	//			IGameMesh* iGameMesh = (IGameMesh*)obj;
	//			iGameMesh->SetCreateOptimizedNormalList();
	//			if(iGameMesh->InitializeData())
	//			{
	//				BOOL result = dumpMesh(iGameMesh, name.data());
	//				if(!result)
	//					return FALSE;
	//			}
	//			break;
	//		}
	//		///< recusive
	//	}
	//}
	/////<
	//int childNodeNum = node->GetChildCount();
	//for(int i = 0; i < childNodeNum; i++)
	//{
	//	IGameNode* newchild = node->GetNodeChild(i);
	//	if(newchild->IsTarget() || newchild->IsNodeHidden())
	//		continue;
	//	BOOL result = exportIGameNode_r(newchild);
	//	if(!result)
	//		return FALSE;
	//}

	//node->ReleaseIGameObject();
	return TRUE;
}

BOOL kiMeshUtility::dumpMesh(vector<finalVex>& vexs, vector<unsigned short>& indices, IGameMesh* iGameMesh, BOOL& skined, const char* name)
{
	int faceNum			= iGameMesh->GetNumberOfFaces();
	int vertexNum		= iGameMesh->GetNumberOfVerts();
	int tangentNum		= iGameMesh->GetNumberOfTangents();
	int binormalNum		= iGameMesh->GetNumberOfBinormals();
	int normalNum		= iGameMesh->GetNumberOfNormals();
	int clr0Num			= iGameMesh->GetNumberOfColorVerts();
	int alphaNum		= iGameMesh->GetNumberOfAlphaVerts();
	int mapChannel0		= iGameMesh->GetNumberOfMapVerts(0);

	struct BoneInfo
	{
		short id[3];
		float weight[3];
	};

	struct BI
	{
		short id;
		float weight;
	};

	vector<Point3>	posArray;
	vector<Point3>	clr0Array;
	vector<Point3>	norArray;
	vector<Point3>	tgtArray;
	vector<Point3>	binArray;
	vector<Point2>	uv0Array;
	vector<BoneInfo> boneInfoArray;
	vector<float>	alphaArray;

	posArray.resize(vertexNum);
	clr0Array.resize(clr0Num);
	norArray.resize(normalNum);
	tgtArray.resize(tangentNum);
	binArray.resize(binormalNum);
	uv0Array.resize(mapChannel0);
	uv0Array.resize(mapChannel0);
	alphaArray.resize(alphaNum);
	boneInfoArray.resize(vertexNum);

	float alpha = 0.f;
	for(int i = 0; i < alphaNum; i++)
	{
		if(iGameMesh->GetAlphaVertex(i, alpha))
			alphaArray[i] = alpha;
	}
	
	Point3 v; 
	for(int i = 0; i < vertexNum; i++)
	{
		if(iGameMesh->GetVertex(i, v, false))
		{
			if(v.x > 16.f || v.y > 16.f || v.z > 16.f)
			{
				::MessageBox(NULL, TEXT("position number larger than 16.0."), name, MB_ICONERROR);
				return FALSE;	///< 防止float32->float16精度损失过大
			}
			posArray[i] = v;
		}
	}
		
	for(int i = 0; i < clr0Num; i++)
	{
		if(iGameMesh->GetColorVertex(i, v))
			clr0Array[i] = v;
	}

	for(int i = 0;i < normalNum; i++)
	{
		if(iGameMesh->GetNormal(i, v, false))
			norArray[i] = v;
	}

	for(int i = 0; i < tangentNum; i++)
	{
		if(iGameMesh->GetTangent(i, v, 1))
			tgtArray[i] = v;
	}

	for(int i = 0; i < binormalNum; i++)
	{
		if(iGameMesh->GetBinormal(i, v, 1))
			binArray[i] = v;
	}

	Point2 uv0;
	for(int i = 0; i < mapChannel0; i++)
	{
		if(iGameMesh->GetTexVertex(i, uv0))
			uv0Array[i] = uv0;
	}

#define MAX_BONE_PER_VEX 16
	vector<BI>	bones;
	bones.resize(MAX_BONE_PER_VEX);

	IGameObject* obj = (IGameObject*)iGameMesh;
	for(int i = 0 ;i < obj->GetNumModifiers(); i++)
	{
		IGameModifier* modifier = obj->GetIGameModifier(i);
		if(modifier->IsSkin())
		{
			IGameSkin* skin = (IGameSkin*)modifier;
			for (int v = 0; v < skin->GetNumOfSkinnedVerts(); v++)
			{
				///< 获得每个顶点受影响的骨骼
				int boneNum = skin->GetNumberOfBones(v);
				if(boneNum > MAX_BONE_PER_VEX)
				{
					///< 有顶点受超过16根骨骼影响
					::MessageBox(NULL, TEXT("有顶点受超过16根骨骼影响"), name, MB_ICONERROR);
					return FALSE;
				}

				for (int b = 0; b < boneNum; b++)
				{
					int id			= skin->GetBoneID(v, b);
					if(id >= 1024)
					{
						::MessageBox(NULL, TEXT("骨架超过1024跟骨骼"), name, MB_ICONERROR);
						return FALSE;
					}
					float weight	= skin->GetWeight(v, b);
					bones[b].id		= id;
					bones[b].weight	= weight;
				}

				///< 骨骼按权重从大到小排列
				BI temp;
				for(int k = 0; k < boneNum; k++)
				{
					int tmp = boneNum;
					for(int j = 0; j < tmp - 1; j++)
					{
						if(bones[j].weight < bones[j+1].weight)
						{
							temp = bones[j];
							bones[j] = bones[j+1];
							bones[j+1] = temp;
						}
					}
					tmp--;
				}

				///< d3d限制，每个顶点骨骼最好不要超过3根
				if(boneNum > 3)
				{
					///< 有顶点受超过4根骨骼影响
					::MessageBox(NULL, TEXT("有顶点受超过3根骨骼影响"), name, MB_ICONERROR);
					//boneNum = 3;
					return FALSE;
				}

				for(int s = 0; s < boneNum; s++)
				{
					boneInfoArray[v].id[s] = bones[s].id;
					boneInfoArray[v].weight[s] = (bones[s].weight > 0.00001) ? bones[s].weight : 0.0f;
				}
				skined = true;
			}
		}
		///< skin modifier end
	}

	///< 3dsmax中用面来构造几何体
	vector<vexRef> vexTotal;
	for(int n = 0;n < faceNum; n++)
	{
		FaceEx* face = iGameMesh->GetFace(n);
		for(int k = 0; k < 3; k++)
		{
			vexRef vref;
			vref.posRef		= face->vert[k];
			vref.norRef		= face->norm[k];
			vref.tex0Ref	= face->texCoord[k];
			vref.color0Ref	= face->color[k];
			vref.alphaRef	= face->alpha[k];
			vref.tbRef		= iGameMesh->GetFaceVertexTangentBinormal(n, k);

			if(skined)
			{
				for(int b = 0; b < 3; b++)
				{
					vref.boneIndex[b]	= boneInfoArray[vref.posRef].id[b];
					vref.boneWeight[b]	= boneInfoArray[vref.posRef].weight[b];
				}
			}
			vexTotal.push_back(vref);
		}		
	}

	vector<vexRef>	vexCompact;
	compactVex(vexTotal, vexCompact, indices);
	///< 检查索引合法性
	UINT totalVex = (UINT)vexCompact.size();
	if(totalVex > 65536)
	{
		::MessageBox(NULL, TEXT("有顶点数量超过65536"), name, MB_ICONERROR);
		return FALSE;
	}
	UINT indexNum = (UINT)indices.size();
	for (UINT i = 0; i < indexNum; i++)
	{
		if((UINT)indices[i] >= totalVex)
		{
			::MessageBox(NULL, TEXT("索引值错误"), name, MB_ICONERROR);
			return FALSE;
		}
	}

	vector<finalVex>& vb = vexs;
	vb.resize(totalVex);

	Point3 nv, tv, c0v, biv, temp;
	float fff = 0;
	unsigned short bsign = 0;
	for (int i = 0; i < (int)vexCompact.size(); ++i)
	{
		v = posArray[vexCompact[i].posRef];
		nv = norArray[vexCompact[i].norRef];
		nv = nv.Normalize();
		tv = tgtArray[vexCompact[i].tbRef];
		tv = tv.Normalize();
		biv = binArray[vexCompact[i].tbRef];

		if(clr0Array.size() > 0)
			c0v = clr0Array[vexCompact[i].color0Ref];
		else
			c0v = Point3(0, 0, 0);

		if(uv0Array.size() > 0)
			uv0 = uv0Array[vexCompact[i].tex0Ref];
		else
			uv0 = Point2(0, 0);

		if(alphaArray.size() > 0)
			alpha = alphaArray[vexCompact[i].alphaRef];
		else
			alpha = 0.f;

		temp = nv ^ tv;
		fff = temp % biv;
		if(fff < 0.f)
			bsign = 0;
		else
			bsign = 2; ///< shader中还原时 intput.pos.w-1 就OK了

		///< position
		vb[i].pos[0] = float32Tofloat16(v.x);
		vb[i].pos[1] = float32Tofloat16(v.y);
		vb[i].pos[2] = float32Tofloat16(v.z);	////<xyz
		vb[i].pos[3] = bsign;						///< n/a

		///< normal
		vb[i].nor[0] = float32Tofloat16(nv.x);
		vb[i].nor[1] = float32Tofloat16(nv.y);
		vb[i].nor[2] = float32Tofloat16(nv.z);

		///< tangent
		vb[i].tgt[0] = float32Tofloat16(tv.x);
		vb[i].tgt[1] = float32Tofloat16(tv.y);
		vb[i].tgt[2] = float32Tofloat16(tv.z);

		///< uv0
		vb[i].nor[3] = float32Tofloat16(uv0.x);
		vb[i].tgt[3] = float32Tofloat16(1.0f - uv0.y);	///< 这里很奇怪为什么要1.0f-y

#define COMPACT_ARGB(a,r,g,b) \
	((UINT)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
		///< colora
		//vb[i].clr0 = COMPACT_ARGB((unsigned char)alpha*255, (unsigned char)c0v.x*255, (unsigned char)c0v.y*255, (unsigned char)c0v.z*255);

		if(skined)
		{
			vb[i].bi = ((vexCompact[i].boneIndex[0] & 0x000003ff) | \
						((vexCompact[i].boneIndex[1] << 10) & 0x000ffc00) | \
						((vexCompact[i].boneIndex[2] << 20) & 0x3ff00000) | \
						((0 << 30) & 0xc0000000));

			vb[i].bw =  (																				\
						 (((UINT)(vexCompact[i].boneWeight[0]*1024.f + 0.5f))        & 0x000003ff) |	\
						((((UINT)(vexCompact[i].boneWeight[1]*1024.f + 0.5f)) << 10) & 0x000ffc00) |	\
						((((UINT)(vexCompact[i].boneWeight[2]*1024.f + 0.5f)) << 20) & 0x3ff00000)		\
						);
		}
		else
		{
			vb[i].bi = vb[i].bw = 0;
		}
	}

	return TRUE;
}

unsigned short kiMeshUtility::float32Tofloat16(float f)
{
	unsigned int Result;

	unsigned int IValue = ((unsigned int *)(&f))[0];
	unsigned int Sign = (IValue & 0x80000000U) >> 16U;
	IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

	if (IValue > 0x47FFEFFFU)
	{
		// The number is too large to be represented as a half.  Saturate to infinity.
		Result = 0x7FFFU;
	}
	else
	{
		if (IValue < 0x38800000U)
		{
			// The number is too small to be represented as a normalized half.
			// Convert it to a denormalized value.
			unsigned int Shift = 113U - (IValue >> 23U);
			IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
		}
		else
		{
			// Rebias the exponent to represent the value as a normalized half.
			IValue += 0xC8000000U;
		}

		Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U)&0x7FFFU; 
	}
	return (unsigned short)(Result|Sign);
}

void kiMeshUtility::compactVex(vector<vexRef>& orgin, vector<vexRef>& compact, vector<unsigned short>& index)
{
	for(int i = 0; i < (int)orgin.size(); i++)
	{
		bool same=false;
		for(int j = 0; j < (int)compact.size(); j++)
		{
			if(orgin[i].posRef == compact[j].posRef && orgin[i].norRef == compact[j].norRef && \
				orgin[i].tex0Ref == compact[j].tex0Ref && orgin[i].tbRef ==compact[j].tbRef)
			{
				same=true;
				index.push_back((unsigned short)j);
				break;
			}
		}
		if(!same)
		{
			compact.push_back(orgin[i]);
			index.push_back((unsigned short)compact.size()-1);
		}
	}

	orgin.clear();
}
#include "kiStaticSolid.h"
#include "kiRenderer.h"

IMPL_DYNAMIC(kiStaticSolid, kiSolid)

kiStaticSolid::kiStaticSolid()
: mVB(NULL)
, mIB(NULL)
{

}

kiStaticSolid::~kiStaticSolid()
{

}

BOOL kiStaticSolid::init(VOID* initData, UINT size)
{
	struct structData
	{
		VOID* vexData;
		UINT vexSize;
		VOID* indexData;
		UINT indexSize;
	};
	ID3D11Device* d3d11 = kiRenderer::getInstance()->getD3D11Device();

	if(size != sizeof(structData) || d3d11 == NULL)
		return FALSE;

	structData* data = (structData*)initData;
	if(mVB != NULL || d3d11 == NULL || mIB != NULL)
		__asm int 3;

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth	= data->vexSize;
	bd.Usage		= D3D11_USAGE_IMMUTABLE;
	bd.BindFlags	= D3D11_BIND_VERTEX_BUFFER/* | D3D11_BIND_SHADER_RESOURCE*/;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags	= 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA dsd;
	dsd.pSysMem = data->vexData;
	dsd.SysMemPitch = 0;
	dsd.SysMemSlicePitch = 0;

	HRESULT result = d3d11->CreateBuffer(&bd, &dsd, &mVB);
	if(result != S_OK)
	{
		finl();
		return FALSE;
	}

	bd.ByteWidth = data->indexSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	dsd.pSysMem = data->indexData;
	result = d3d11->CreateBuffer(&bd, &dsd, &mIB);
	if(result != S_OK)
	{
		finl();
		return FALSE;
	}

	return TRUE;
}

VOID kiStaticSolid::finl()
{
	if(mVB != NULL)
	{
		UINT refCount = mVB->Release();
		if(refCount != 0)
			__asm int 3;
		mVB = NULL;
	}

	if(mIB != NULL)
	{
		UINT refCount = mIB->Release();
		if(refCount != 0)
			__asm int 3;
		mIB = NULL;
	}
}
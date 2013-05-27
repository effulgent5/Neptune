#include "kiStateFactory.h"
#include "kiRenderer.h"
#include "kiShaderPool.h"

kiStateFactory* kiStateFactory::ms_instance = NULL;

kiStateFactory::kiStateFactory()
{
	memset(mLayoutState, 0, sizeof(mLayoutState));
	memset(mSampState, 0, sizeof(mSampState));
	memset(mRastState, 0, sizeof(mRastState));
	memset(mBlendState, 0, sizeof(mBlendState));
	memset(mDSState, 0, sizeof(mDSState));
}

kiStateFactory::~kiStateFactory()
{

}

BOOL kiStateFactory::initLayoutState(ID3D11Device* device)
{
	kiShaderPool* pool = kiShaderPool::getInstance();
	if(pool == NULL)
		return FALSE;

	kiShader* layoutShader = pool->getShader(0);
	if(layoutShader == NULL)
		return FALSE;

	ID3DBlob* blob = layoutShader->getBytecode(0);
	if(blob == NULL)
		return FALSE;	

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "SV_POSITION", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32_UINT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32_UINT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElement = ARRAYSIZE(layout);
	ID3D11InputLayout* solidLayout = NULL;
	HRESULT hr = device->CreateInputLayout(layout, numElement, blob->GetBufferPointer(), blob->GetBufferSize(), &mLayoutState[(UINT)LYT_0]);

	return TRUE;
}

BOOL kiStateFactory::initSampState(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC sampDesc0 = 
		{D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, \
			0.f, 16, D3D11_COMPARISON_NEVER, {1.f, 0.f, 1.f, 1.f}, -FLT_MAX, FLT_MAX};
	HRESULT hr = device->CreateSamplerState(&sampDesc0, &mSampState[(UINT)SAT_LINEAR]);

	D3D11_SAMPLER_DESC sampDesc1 = 
	{D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, \
	0.f, 16, D3D11_COMPARISON_NEVER, {1.f, 0.f, 1.f, 1.f}, -FLT_MAX, FLT_MAX};
	hr = device->CreateSamplerState(&sampDesc1, &mSampState[(UINT)SAT_POINT]);

	D3D11_SAMPLER_DESC sampDesc2 = 
	{D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, \
	0.f, 16, D3D11_COMPARISON_NEVER, {1.f, 0.f, 1.f, 1.f}, -FLT_MAX, FLT_MAX};
	hr = device->CreateSamplerState(&sampDesc2, &mSampState[(UINT)SAT_ANI]);

	return TRUE;
}

BOOL kiStateFactory::initRastState(ID3D11Device* device)
{
	HRESULT hr = S_FALSE;
	D3D11_RASTERIZER_DESC rastDesc0 = {D3D11_FILL_SOLID, D3D11_CULL_BACK, FALSE, 0, 0.f, 0.f, TRUE, FALSE, FALSE, FALSE};
	hr = device->CreateRasterizerState(&rastDesc0, &mRastState[RAT_SOLID]);

	D3D11_RASTERIZER_DESC rastDesc1 = {D3D11_FILL_WIREFRAME, D3D11_CULL_BACK, FALSE, 0, 0.f, 0.f, TRUE, FALSE, FALSE, FALSE};
	hr = device->CreateRasterizerState(&rastDesc1, &mRastState[RAT_WIREFRAME]);

	return TRUE;
}

BOOL kiStateFactory::initBlendState(ID3D11Device* device)
{
	HRESULT hr = S_FALSE;
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	D3D11_RENDER_TARGET_BLEND_DESC rtbDesc0 = {FALSE, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, \
												D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL};
	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		blendDesc.RenderTarget[i] = rtbDesc0;
	hr = device->CreateBlendState(&blendDesc, &mBlendState[BDT_DISABLE]);

	D3D11_RENDER_TARGET_BLEND_DESC rtbDesc1 = {TRUE, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, \
												D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL};
	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		blendDesc.RenderTarget[i] = rtbDesc1;
	hr = device->CreateBlendState(&blendDesc, &mBlendState[BDT_SRCINVSRC]);

	D3D11_RENDER_TARGET_BLEND_DESC rtbDesc2 = {TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, \
												D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL};
	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		blendDesc.RenderTarget[i] = rtbDesc2;
	hr = device->CreateBlendState(&blendDesc, &mBlendState[BDT_ONEONE]);
	
	return hr;
}

BOOL kiStateFactory::initDSState(ID3D11Device* device)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc0 = \
				{TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, FALSE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, \
				{D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS}, \
				{D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS}};

	HRESULT hr = S_FALSE;
	hr = device->CreateDepthStencilState(&dsDesc0, &mDSState[DST_ZE_SD]);

	D3D11_DEPTH_STENCIL_DESC dsDesc1 = \
				{FALSE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, FALSE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, \
				{D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS}, \
				{D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS}};
	hr = device->CreateDepthStencilState(&dsDesc1, &mDSState[DST_ZD_SD]);

	D3D11_DEPTH_STENCIL_DESC dsDesc2 = \
	{TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, TRUE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, \
	{D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS}, \
	{D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS}};
	hr = device->CreateDepthStencilState(&dsDesc2, &mDSState[DST_ZE_SREPLACE]);

	return TRUE;
}

BOOL kiStateFactory::init()
{
	HRESULT hr = S_FALSE;
	kiRenderer* renderer = kiRenderer::getInstance();
	if(renderer == NULL)
		return FALSE;

	ID3D11Device* device = renderer->getD3D11Device();
	if(device == NULL)
		return FALSE;

	hr = initLayoutState(device);
	hr = initSampState(device);
	hr = initRastState(device);
	hr = initBlendState(device);
	hr = initDSState(device);

	ms_instance = this;

	return TRUE;
}

#define RELEASE_D3D11_STATE(state, count) \
	for(int i = 0; i < (int)count; ++i) { if(state[i] = NULL) { state[i]->Release(); state[i] = NULL; } }

VOID kiStateFactory::finl()
{
	ms_instance = NULL;

	RELEASE_D3D11_STATE(mLayoutState, LYT_TOTAL);
	RELEASE_D3D11_STATE(mSampState, SAT_TOTAL);
	RELEASE_D3D11_STATE(mRastState, RAT_TOTAL);
	RELEASE_D3D11_STATE(mBlendState, BDT_TOTAL);
	RELEASE_D3D11_STATE(mDSState, DST_TOTAL);
}

ID3D11InputLayout* kiStateFactory::getInputLayout(eLayoutType type)
{
	return mLayoutState[(UINT)type];
}

ID3D11SamplerState* kiStateFactory::getSampState(eSampType type)
{
	return mSampState[(UINT)type];
}

ID3D11RasterizerState* kiStateFactory::getRastState(eRastType type)
{
	return mRastState[(UINT)type];
}

ID3D11BlendState* kiStateFactory::getBlendState(eBlendType type)
{
	return mBlendState[(UINT)type];
}

ID3D11DepthStencilState* kiStateFactory::getDSState(eDSType type)
{
	return mDSState[(UINT)type];
}
#include "kiGraphicPipeline.h"
#include "kiStepBase.h"
#include "kiGeoStep.h"
#include "kiFinalStep.h"

kiGraphicPipeline* kiGraphicPipeline::ms_pipeline = NULL;

kiGraphicPipeline* kiGraphicPipeline::getInstance()
{
	return ms_pipeline;
}

kiGraphicPipeline::kiGraphicPipeline()
: mTex_10_10_10_2(NULL)
, mTex_32_8_0(NULL)
{
	//mVexLayout = NULL;
	memset(mSteps, 0, sizeof(mSteps));
}

kiGraphicPipeline::~kiGraphicPipeline()
{

}

BOOL kiGraphicPipeline::init(UINT w, UINT h, ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_FALSE;
	mDevice = device;

	ms_pipeline = this;


	//HRESULT hr = context->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), &mMarker);
	
	///< 创建公用渲染缓冲资源
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width			= w;
	texDesc.Height			= h;
	texDesc.MipLevels		= 1;
	texDesc.ArraySize		= 1;
	texDesc.Format			= DXGI_FORMAT_R10G10B10A2_TYPELESS;
	texDesc.SampleDesc.Count	= 1;	///<不使用抗锯齿
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage			= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags		= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags	= 0;
	texDesc.MiscFlags		= 0 /*D3D11_RESOURCE_MISC_RESOURCE_CLAMP*/;
	hr = mDevice->CreateTexture2D(&texDesc, 0, &mTex_10_10_10_2);
	if(hr != S_OK) goto _INIT_ERROR;

	///< 创建深度模版缓冲
	texDesc.Format			= DXGI_FORMAT_R32G8X24_TYPELESS;
	texDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	hr = mDevice->CreateTexture2D(&texDesc, 0, &mTex_32_8_0);
	if(hr != S_OK) goto _INIT_ERROR;

	//D3D11_RENDER_TARGET_VIEW_DESC rtViewDesc;
	//rtViewDesc.Format		= texDesc.Format;
	//rtViewDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;
	//rtViewDesc.Texture2D.MipSlice = 0;
	hr = mDevice->CreateRenderTargetView(mTex_10_10_10_2, NULL, &mRTView);
	if(hr != S_OK) goto _INIT_ERROR;


	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
	dsViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Flags = 0;
	dsViewDesc.Texture2D.MipSlice = 0;
	///D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
	hr = mDevice->CreateDepthStencilView(mTex_32_8_0, &dsViewDesc, &mDSView);
	if(hr != S_OK) goto _INIT_ERROR;

	ID3D11ShaderResourceView* dsSRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;//DXGI_FORMAT_R32G8X24_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	hr = mDevice->CreateShaderResourceView(mTex_32_8_0, &srvDesc, &dsSRV);
	if(hr != S_OK) goto _INIT_ERROR;

	initStep();

	return TRUE;
_INIT_ERROR:
	finl();
	return FALSE;
}

VOID kiGraphicPipeline::updatePerFrameCB()
{

}

VOID kiGraphicPipeline::finl()
{
	finlStep();

	ms_pipeline = NULL;
}

BOOL kiGraphicPipeline::initStep()
{
	mSteps[(UINT)SP_GEOM] = kiNew1 kiGeoStep((UINT)SP_GEOM);
	mSteps[(UINT)SP_FINL] = kiNew1 kiFinalStep((UINT)SP_FINL);
	return TRUE;
}

VOID kiGraphicPipeline::finlStep()
{
	for (UINT i = 0; i < MAX_PIPELINE_STEP; ++i)
	{
		if(mSteps[i] != NULL)
			kiDelete1 mSteps[i];
	}
	memset(mSteps, 0, sizeof(mSteps));
}

VOID kiGraphicPipeline::clear()
{
	for (UINT i = 0; i < MAX_PIPELINE_STEP; ++i)
	{
		if(mSteps[i] != NULL)
			mSteps[i]->clear();
	}
}

VOID kiGraphicPipeline::draw()
{
	updatePerFrameCB();

	//D3DPERF_BeginEvent(0xffff0000, L"Test");
	//D3DPERF_EndEvent();
	///ID3DUserDefinedAnnotation 

	mSteps[(UINT)SP_GEOM]->draw();

	mSteps[(UINT)SP_FINL]->draw();
}
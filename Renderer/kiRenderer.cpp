// kiRenderer.cpp : Defines the exported functions for the DLL application.
//
#include "kiRenderer.h"
#include "kiShaderPool.h"
#include "kiBanal.h"
#include "kiMetal.h"
#include "kiRenderFactory.h"
#include "kiMaterial.h"
#include "kiGraphicPipeline.h"
#include "kiStateFactory.h"

IMPL_DYNAMIC(kiRenderer, kiRenderBase)

struct stRendererData
{
	HWND wnd;
	UINT w;
	UINT h;
};

kiRenderer* kiRenderer::ms_instance = NULL;

kiRenderer::kiRenderer()
{
	mMainHwnd			= NULL;
	mDriverType			= D3D_DRIVER_TYPE_UNKNOWN;
	mD3D11Device		= NULL;
	mFeatureLevel		= (D3D_FEATURE_LEVEL)0;
	mImmDevContext		= NULL;
	mMainSwapChain		= NULL;
	mDXGIFactory		= NULL;
	mCreateThread		= ::GetCurrentThreadId();
	mBackBuffer			= NULL;
	mFinalRTView		= NULL;
	mStateFactory		= NULL;
	mShaderPool			= NULL;
	mPipeline			= NULL;
	memset(mMaterial, 0, sizeof(mMaterial));
};

kiRenderer::~kiRenderer()
{

};

BOOL kiRenderer::init(VOID* initData, UINT size)
{
	g_rendererDB.init();
	stRendererData* data = (stRendererData*)initData;

	ms_instance = this;

	if(sizeof(stRendererData) != size || data->wnd == NULL || data->w <= 0 || data->h <= 0)
		return FALSE;
	HRESULT hr = NULL;
	//IDXGIFactory1* dxgiFactory1;
	//HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&dxgiFactory1));
	//if(hr != S_OK)
	//	return 0;

	//UINT ui = 0; 
	//IDXGIAdapter1* adapter; 
	//std::vector <IDXGIAdapter1*> vAdapters; 
	//DXGI_ADAPTER_DESC1 desc1;
	//while(dxgiFactory1->EnumAdapters1(ui, &adapter) != DXGI_ERROR_NOT_FOUND) 
	//{ 
	//	memset(&desc1, 0, sizeof(desc1));
	//	adapter->GetDesc1(&desc1);
	//	vAdapters.push_back(adapter); 
	//	++ui; 
	//} 
	mMainHwnd = data->wnd;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	for(UINT i = 0; i < numDriverTypes; ++i)
	{
		mDriverType = driverTypes[i];
		hr = D3D11CreateDevice(NULL, mDriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, \
								D3D11_SDK_VERSION, &mD3D11Device, &mFeatureLevel, &mImmDevContext);
		if(hr == S_OK)
			break;
	}

	IDXGIDevice1* mainDXGIDevice;
	hr = mD3D11Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&mainDXGIDevice);
	IDXGIAdapter1* mainAdapter = NULL;
	hr = mainDXGIDevice->GetParent(__uuidof(IDXGIAdapter1), (void**)&mainAdapter);
	mainAdapter->GetParent(__uuidof(IDXGIFactory1), (void**)&mDXGIFactory);
	mainAdapter->Release();
	mainDXGIDevice->Release();

	if(hr != S_OK) goto _INIT_ERROR;

	///< 创建渲染交换链
	DXGI_SWAP_CHAIN_DESC swapChainProp;
	memset(&swapChainProp, 0, sizeof(swapChainProp));
	swapChainProp.BufferCount			= 1;
	swapChainProp.BufferDesc.Width		= data->w;
	swapChainProp.BufferDesc.Height		= data->h;
	swapChainProp.BufferDesc.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainProp.BufferDesc.RefreshRate.Numerator		= 60;
	swapChainProp.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainProp.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainProp.OutputWindow			= mMainHwnd;
	swapChainProp.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;
	swapChainProp.SampleDesc.Count		= 1;
	swapChainProp.SampleDesc.Quality	= 0;
	swapChainProp.Windowed				= TRUE;

	hr = mDXGIFactory->CreateSwapChain(mainDXGIDevice, &swapChainProp, &mMainSwapChain);
	if(hr != S_OK) goto _INIT_ERROR;

	///< 获取交换链中的缓冲
	ID3D11Texture2D* backBuffer = NULL;
	hr = mMainSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mBackBuffer);
	if(hr != S_OK) goto _INIT_ERROR;

	hr = mD3D11Device->CreateRenderTargetView(mBackBuffer, NULL, &mFinalRTView);
	if(hr != S_OK) goto _INIT_ERROR;

	///< 设置视口
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)data->w;
	vp.Height = (FLOAT)data->h;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mImmDevContext->RSSetViewports(1, &vp);

	mShaderPool = kiNew1 kiShaderPool();
	if(mShaderPool == NULL) goto _INIT_ERROR;

	BOOL result = mShaderPool->init();
	if(!result) goto _INIT_ERROR;

	result = initMaterial();
	if(!result) goto _INIT_ERROR;

	mStateFactory = kiNew1 kiStateFactory();
	result = mStateFactory->init();
	if(!result) goto _INIT_ERROR;

	mPipeline = kiNew1 kiGraphicPipeline();
	if(mPipeline == NULL) goto _INIT_ERROR;

	result = mPipeline->init(data->w, data->h, mD3D11Device, mImmDevContext);
	if(!result) goto _INIT_ERROR;

	return TRUE;
	
_INIT_ERROR:
	finl();
	return FALSE;
}

void kiRenderer::finl()
{
	ms_instance = NULL;

	FINL_APP_PT(mPipeline);
	FINL_APP_PT(mShaderPool);
	FINL_APP_PT(mStateFactory);

	finlMaterial();

	RELEASE_APP_RES(mFinalRTView);
	RELEASE_APP_RES(mBackBuffer);
	RELEASE_APP_RES(mDXGIFactory);
	RELEASE_APP_RES(mImmDevContext);
	RELEASE_APP_RES(mMainSwapChain);
	RELEASE_APP_RES(mD3D11Device);

	mMainHwnd			= NULL;
	mDriverType			= D3D_DRIVER_TYPE_UNKNOWN;
	mFeatureLevel		= (D3D_FEATURE_LEVEL)0;
}

BOOL kiRenderer::initMaterial()
{
	mMaterial[kiMaterial::MT_BANL] = kiNew1 kiMaterial('BANL');
	kiMaterial::stMatData md;
	md.shader[kiShader::SD_VS] = 0001;
	md.shader[kiShader::SD_PS] = 0004;
	mMaterial[kiMaterial::MT_BANL]->appendMatData(kiGraphicPipeline::SP_GEOM, md);
	mMaterial[kiMaterial::MT_BANL]->appendMatData(kiGraphicPipeline::SP_FINL, md);

	//mMaterial[kiMaterial::MT_METL] = kiNew1 kiMaterial('METL');
	return TRUE;
}

VOID kiRenderer::finlMaterial()
{
	for(UINT i = 0; i < kiMaterial::MT_TOTAL; ++i)
	{
		if(mMaterial[i] != NULL)
		{
			kiDelete1 mMaterial[i];
			mMaterial[i] = NULL;
		}
	}
}

BOOL kiRenderer::resize(int w, int h)
{
	return FALSE;
}

int kiRenderer::begin(ICamera* camera)
{
	float clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	mImmDevContext->ClearRenderTargetView(mFinalRTView, clearColor);
	mMainSwapChain->Present(0, 0);
	return 0;
}

void kiRenderer::renderBanal(ISolid* s, kiTransform* w, IBanal* param)
{
	if(param != NULL)
	{
		kiMaterial* mat = ((kiBanal*)param)->getMaterial();
		if(mat != NULL)
			mat->insertPipeline((kiSolid*)s, w, (kiBanal*)param);
	}
}

void kiRenderer::renderMetal(ISolid* s, kiTransform* w, IMetal* param)
{
	if(param != NULL)
	{
		kiMaterial* mat = ((kiMetal*)param)->getMaterial();
		if(mat != NULL)
			mat->insertPipeline((kiSolid*)s, w, (kiMetal*)param);
	}
}

int kiRenderer::end()
{
	return 0;
}

IRenderBase* kiRenderer::createItem(const tstring& className, void* initData, UINT size)
{
	kiRenderBase* item = (kiRenderBase*)g_rendererDB.create(className, initData, size);
	IRenderBase* inter = reinterpret_cast<IRenderBase*>(item);
	return inter;
}

extern "C" RENDERER_EI IRenderer* createRenderer(HWND wnd, UINT w, UINT h)
{
	kiRenderer* renderer = kiRenderer::getInstance();
	if(renderer == NULL)
	{
		stRendererData rd = { wnd, w, h };
		renderer = (kiRenderer*)g_rendererDB.create(TEXT("kiRenderer"), &rd, sizeof(stRendererData));
	}
	
	IRenderer* inter = reinterpret_cast<IRenderer*>(renderer);
	return inter;
}

extern "C" RENDERER_EI UINT inspectRendererResource()
{
	UINT leakCount = g_rendererDB.getResourceCount();
	g_rendererDB.finl();
	return leakCount;
}

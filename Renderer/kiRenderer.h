#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderBase.h"
#include "kiMaterial.h"

class kiShaderPool;
class kiTransform;
class kiGraphicPipeline;
class kiStateFactory;
class ICamera;
class ISolid;
class IBanal;
class IMetal;
class IRenderBase;
class IRenderer;

class kiRenderer : public kiRenderBase
{
public:
	kiRenderer();
	virtual ~kiRenderer();

	virtual BOOL KICALL_TYPE init(VOID* initData, UINT size);
	virtual VOID KICALL_TYPE finl();

	///< IRenderer Interface
	virtual BOOL KICALL_TYPE resize(int w, int h);
	virtual INT KICALL_TYPE begin(ICamera* camera);
	virtual VOID KICALL_TYPE renderBanal(ISolid* s, kiTransform* w, IBanal* param);
	virtual VOID KICALL_TYPE renderMetal(ISolid* s, kiTransform* w, IMetal* param);
	virtual INT KICALL_TYPE end();

	virtual IRenderBase* KICALL_TYPE createItem(const tstring& className, void* initData, UINT size);
	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiRenderer)

	inline ID3D11Device* getD3D11Device();
	inline kiMaterial* getMaterial(kiMaterial::eMatType type);
protected:
	BOOL	initMaterial();
	VOID	finlMaterial();

private:
	D3D_DRIVER_TYPE				mDriverType;
	HWND						mMainHwnd;
	ID3D11Device*				mD3D11Device;
	D3D_FEATURE_LEVEL			mFeatureLevel;
	ID3D11DeviceContext*		mImmDevContext;
	IDXGISwapChain*				mMainSwapChain;
	IDXGIFactory1*				mDXGIFactory;
	ID3D11Texture2D*			mBackBuffer;
	ID3D11RenderTargetView*		mFinalRTView;
	UINT						mCreateThread;

	///< pipeline
	kiShaderPool*				mShaderPool;
	kiStateFactory*				mStateFactory;
	kiGraphicPipeline*			mPipeline;
	kiMaterial*					mMaterial[kiMaterial::MT_TOTAL];


	static kiRenderer* ms_instance;
public:
	static kiRenderer* getInstance()
	{
		return ms_instance;
	}
};

inline ID3D11Device* kiRenderer::getD3D11Device()
{
	return mD3D11Device;
}

inline kiMaterial* kiRenderer::getMaterial(kiMaterial::eMatType type)
{
	if(type < kiMaterial::MT_TOTAL)
		return mMaterial[(UINT)type];
	else
		return NULL;
}
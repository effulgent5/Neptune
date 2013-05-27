#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"

///< d3d11��5��state object : 
///< Input-Layout State : ID3D11InputLayout
///< Rasterizer State : ID3D11RasterizerState
///< Depth-Stencil State : ID3D11DepthStencilState
///< Blend State : ID3D11BlendState
///< Sampler State : ID3D11SamplerState
///< ���е�state object���Ǿ�̬�����޸ĵ�
/*
D3D11�D����ˮ����Ԫ�أ�
1��RenderTarget
2��DepthStencil
3��VS HS DS GS PS CS
4��SHADER RESOURCE
5��Blend State��DepthStencil State��Rasterizer State��Sampler State
6��IAInputLayout
7��VB IB Topology
8��Constant Buffer
9��Scissor Rect
10��ViewPort
11��Predicate Counter Query
*/
//D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ÿ��Shader���128��shader resource
//D3D11_PS_CS_UAV_REGISTER_COUNT PS CS���8��UAV
//D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT ÿ��shader���16��sampler
//D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ÿ��shader���14��constant buffer
//D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
//D3D11��4��VIEW DSV RTV SRV UAV
#define MAX_PIPELINE_STEP		32

class kiStepBase;

class kiGraphicPipeline : public kiMemObject
{
public:
	kiGraphicPipeline();
	~kiGraphicPipeline();

	enum eRenderStep
	{
		SP_GEOM = 0,
		SP_FINL = 1,
		SP_TOTAL = 2,
		SP_INVALID = 3
	};

	BOOL		init(UINT w, UINT h, ID3D11Device* device, ID3D11DeviceContext* context);
	VOID		finl();

	VOID		draw();

	VOID		clear();
	inline kiStepBase* getStep(UINT stepID);
protected:
	BOOL		initStep();
	VOID		finlStep();

	VOID		updatePerFrameCB();
protected:

	ID3D11Texture2D*			mTex_10_10_10_2;
	ID3D11Texture2D*			mTex_32_8_0;


	ID3D11RenderTargetView*		mRTView;

	///< ���ģ�滺��
	ID3D11DepthStencilView*		mDSView;

	ID3D11Device*				mDevice;
	ID3D11DeviceContext*		mContext;

	kiStepBase*					mSteps[MAX_PIPELINE_STEP];
private:
	static kiGraphicPipeline*	ms_pipeline;
public:
	static kiGraphicPipeline* getInstance();
};

inline kiStepBase* kiGraphicPipeline::getStep(UINT stepID)
{
	if(stepID >= MAX_PIPELINE_STEP)
		__asm int 3;

	return mSteps[stepID];
}
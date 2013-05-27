#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"

class kiStateFactory : public kiMemObject
{
public:
	kiStateFactory();
	~kiStateFactory();

	enum eStateType
	{
		STT_LAYOUT = 0,
		STT_SAMP = 1,
		STT_RAST = 2,
		STT_BLEND = 3,
		STT_DS = 4,
		STT_TOTAL = 5,
		STT_INVALID = 6
	};

	enum eLayoutType
	{
		LYT_0 = 0,
		LYT_1 = 1,
		LYT_2 = 2,
		LYT_TOTAL = 3,
		LYT_INVALID = 4
	};

	enum eSampType
	{
		SAT_LINEAR = 0,
		SAT_POINT = 1,
		SAT_ANI = 2,
		SAT_TOTAL = 3,
		SAT_INVALID = 4
	};

	enum eRastType
	{
		RAT_SOLID = 0,
		RAT_WIREFRAME = 1,
		RAT_MSAA = 2,
		RAT_TOTAL = 3,
		RAT_INVALID = 4
	};

	enum eBlendType
	{
		BDT_DISABLE = 0,
		BDT_SRCINVSRC = 1,
		BDT_ONEONE = 2,
		BDT_TOTAL = 3,
		BDT_INVALID = 4
	};

	enum eDSType
	{
		DST_ZE_SD = 0,
		DST_ZD_SD = 1,
		DST_ZE_SREPLACE = 2,
		DST_TOTAL = 3,
		DST_INVALID = 4
	};

	BOOL init();
	VOID finl();

	ID3D11InputLayout* getInputLayout(eLayoutType type);
	ID3D11SamplerState* getSampState(eSampType type);
	ID3D11RasterizerState* getRastState(eRastType type);
	ID3D11BlendState* getBlendState(eBlendType type);
	ID3D11DepthStencilState* getDSState(eDSType type);
protected:
	BOOL initLayoutState(ID3D11Device* device);
	BOOL initSampState(ID3D11Device* device);
	BOOL initRastState(ID3D11Device* device);
	BOOL initBlendState(ID3D11Device* device);
	BOOL initDSState(ID3D11Device* device);
protected:
	ID3D11InputLayout* mLayoutState[LYT_TOTAL];
	ID3D11SamplerState* mSampState[SAT_TOTAL];
	ID3D11RasterizerState* mRastState[RAT_TOTAL];
	ID3D11BlendState* mBlendState[BDT_TOTAL];
	ID3D11DepthStencilState* mDSState[DST_TOTAL];

	static kiStateFactory* ms_instance;
public:
	static kiStateFactory* getInstance()
	{
		return ms_instance;
	}
};
#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderBase.h"
#include "kiGraphicPipeline.h"
#include "kiShader.h"

class kiSolid;
class kiTransform;
class kiRenderParam;

class kiMaterial : public kiMemObject
{
public:
	kiMaterial(UINT matID);
	virtual ~kiMaterial();

	enum eMatType
	{
		MT_BANL = 0,
		MT_METL = 1,
		MT_TOTAL = 2,
		MT_INVALID = 3
	};

	struct stMatData
	{
		UINT shader[kiShader::SD_TOTAL];
	};

	VOID appendMatData(UINT stepID, const stMatData& matData);
	inline UINT getMatID();

	VOID* getD3D11Shader(UINT stepID, UINT matFlag, kiShader::eShaderType shaderType);
	VOID insertPipeline(kiSolid* s, kiTransform* w, kiRenderParam* p);
protected:

	stMatData		mMatDB[MAX_PIPELINE_STEP];
	UINT			mStepIndex[MAX_PIPELINE_STEP];
	UINT			mMatID;
public:
	kiMaterial* mPre;
	kiMaterial* mNext;
	static kiMaterial*	ms_firstMaterial;
};

inline UINT kiMaterial::getMatID()
{
	return mMatID;
}

#include "kiMaterial.h"
#include "kiShaderPool.h"
#include "kiStepBase.h"

kiMaterial*	kiMaterial::ms_firstMaterial = NULL;

kiMaterial::kiMaterial(UINT matID)
: mMatID(matID)
{
	memset(mMatDB, 0xff, sizeof(mMatDB));
	memset(mStepIndex, 0xff, sizeof(mStepIndex));

	if(ms_firstMaterial == NULL)
	{
		ms_firstMaterial = this;
		mPre = NULL;
		mNext = NULL;
	}
	else
	{
		mNext = ms_firstMaterial;
		ms_firstMaterial->mPre = this;
		mPre = NULL;
		ms_firstMaterial = this;
	}
}

kiMaterial::~kiMaterial()
{

}

VOID kiMaterial::appendMatData(UINT stepID, const kiMaterial::stMatData& matData)
{
	if(stepID >= MAX_PIPELINE_STEP)
		__asm int 3;

	if(mMatDB[stepID].shader[kiShader::SD_VS] == INVALID_SHADER_ID)
	{
		mMatDB[stepID] = matData;
		for (UINT i = 0; i < MAX_PIPELINE_STEP; ++i)
		{
			if(mStepIndex[i] == INVALID_SHADER_ID)
			{
				mStepIndex[i] = stepID;
				break;
			}
		}
	}
	else
		__asm int 3;
}

VOID* kiMaterial::getD3D11Shader(UINT stepID, UINT matFlag, kiShader::eShaderType shaderType)
{
	kiShaderPool* pool = kiShaderPool::getInstance();
	if(pool != NULL)
	{
		UINT shaderID = mMatDB[stepID].shader[(UINT)shaderType];
		if(shaderID != INVALID_SHADER_ID)
		{
			kiShader* shader = pool->getShader(shaderID);
			if(shader != NULL)
			{
				void* d3d11Shader = shader->getD3D11Shader(matFlag);
				if(d3d11Shader != NULL)
					return d3d11Shader;
			}
		}
		///<
	}
	
	return NULL;
}

VOID kiMaterial::insertPipeline(kiSolid* s, kiTransform* w, kiRenderParam* p)
{
	for (UINT i = 0; i < MAX_PIPELINE_STEP; ++i)
	{
		if(mStepIndex[i] != INVALID_SHADER_ID && mStepIndex[i] < MAX_PIPELINE_STEP)
		{
			kiGraphicPipeline* pipeline = kiGraphicPipeline::getInstance();
			if(pipeline != NULL)
			{
				kiStepBase* step = pipeline->getStep(mStepIndex[i]);
				if(step != NULL)
					step->insert(s, w, p);
			}
			///<
		}
		else
		{
			break;
		}
	}
}
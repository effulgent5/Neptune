#include "kiShader.h"
#include "kiShaderPool.h"
#include "kiRenderer.h"
#include <d3d11shader.h>


kiShader::kiShader(kiShader::eShaderType type, void* code, UINT codeSize)
	: mContent(code)
	, mShaderType(type)
	, mCodeSize(codeSize)
{

}

kiShader::~kiShader()
{
}

VOID* kiShader::getD3D11Shader(UINT matFlag)
{
	map<UINT, VOID*>::iterator iter = mD3D11ShaderDB.find(matFlag);
	if(iter == mD3D11ShaderDB.end())
	{
		ID3DBlob* bytecode = NULL;
		VOID* shader = compilerShader(matFlag, bytecode);
		mD3D11ShaderDB[matFlag] = shader;			///< NULL也需要存入
		mBytecodeDB[matFlag] = bytecode;
		return shader;
	}
	else
	{
		return iter->second;
	}
}

ID3DBlob* kiShader::getBytecode(UINT matFlag)
{
	map<UINT, ID3DBlob*>::iterator iter = mBytecodeDB.find(matFlag);
	if(iter == mBytecodeDB.end())
	{
		ID3DBlob* bytecode = NULL;
		VOID* shader = compilerShader(matFlag, bytecode);
		mD3D11ShaderDB[matFlag] = shader;			///< NULL也需要存入
		mBytecodeDB[matFlag] = bytecode;
		return bytecode;
	}
	else
	{
		return iter->second;
	}
}

kiShader::eShaderType kiShader::getShaderType()
{
	return mShaderType;
}

//BOOL kiShader::init(ID3D11ShaderReflection* reflector, ID3D11ClassLinkage* linkage)
//{
//	HRESULT hr = S_FALSE;
//
//	D3D11_SHADER_DESC shaderDesc;
//	ZeroMemory(&shaderDesc, sizeof(shaderDesc));
//	hr = reflector->GetDesc(&shaderDesc);
//
//	UINT num = reflector->GetNumInterfaceSlots();
//
//	//ID3D11ClassInstance** linkageArray = (ID3D11ClassInstance**)kiAlloc1(sizeof(ID3D11ClassInstance*) * num);
//
//	ID3D11ShaderReflectionVariable* lightVar = reflector->GetVariableByName("g_iLight");
//	D3D11_SHADER_VARIABLE_DESC vdesc;
//	memset(&vdesc, 0, sizeof(D3D11_SHADER_VARIABLE_DESC));
//	hr = lightVar->GetDesc(&vdesc);
//
//	ID3D11ShaderReflectionVariable* matVar = reflector->GetVariableByName("g_iMaterial");
//	UINT arrayOffset0 = lightVar->GetInterfaceSlot(0);
//	UINT arrayOffset1 = matVar->GetInterfaceSlot(0);
//
//	ID3D11ClassInstance* ambLight = NULL;
//	hr = linkage->GetClassInstance("g_ambLight", 0, &ambLight);
//
//	ID3D11ClassInstance* dirLight = NULL;
//	hr = linkage->GetClassInstance("g_dirLight", 0, &dirLight);
//
//	ID3D11ClassInstance* baseMat = NULL;
//	hr = linkage->GetClassInstance("g_baseMat", 0, &baseMat);
//
//	ID3D11ClassInstance* texturedMat = NULL;
//	hr = linkage->GetClassInstance("g_texturedMat", 0, &texturedMat);
//	return TRUE;
//}
BOOL kiShader::init()
{
	return TRUE;
}

VOID kiShader::finl()
{
	map<UINT, VOID*>::iterator iter = mD3D11ShaderDB.begin();
	for ( ; iter != mD3D11ShaderDB.end(); ++iter)
	{
		if(iter->second != NULL)
		{
			switch(mShaderType)
			{
			case kiShader::SD_VS:
				((ID3D11VertexShader*)(iter->second))->Release();
				break;
			case kiShader::SD_HS:
				((ID3D11HullShader*)(iter->second))->Release();
				break;
			case kiShader::SD_DS:
				((ID3D11DomainShader*)(iter->second))->Release();
				break;
			case kiShader::SD_GS:
				((ID3D11GeometryShader*)(iter->second))->Release();
				break;
			case kiShader::SD_PS:
				((ID3D11PixelShader*)(iter->second))->Release();
				break;
			case kiShader::SD_CS:
				((ID3D11ComputeShader*)(iter->second))->Release();
				break;
			default:
				break;
			}
		}
	}
	mD3D11ShaderDB.clear();

	map<UINT, ID3DBlob*>::iterator iter1 = mBytecodeDB.begin();
	for ( ; iter1 != mBytecodeDB.end(); ++iter1)
	{
		if(iter1->second != NULL)
			iter1->second->Release();
	}
	mBytecodeDB.clear();

	if(mContent != NULL)
	{
		kiFree1(mContent);
		mContent = NULL;
	}

	mShaderType = SD_INVALID;
	mCodeSize = 0;
}

VOID* kiShader::compilerShader(UINT matFlag, ID3DBlob*& bytecode)
{
	kiShaderPool* pool = kiShaderPool::getInstance();
	if(pool == NULL || mShaderType == SD_INVALID || mContent == NULL || mCodeSize == 0)
		return NULL;

	shader_comiler compiler = pool->getCompiler();
	shader_reflect reflect = pool->getReflect();
	kiShaderInclude* shaderInclude = pool->getShaderInclude();

	if(compiler == NULL || reflect == NULL || shaderInclude == NULL)
		return NULL;

	char macroValue[11];
	memset(macroValue, 0, 11);
	sprintf_s(macroValue, 11, "0x%08X", matFlag);
	const char* target[kiShader::SD_TOTAL] = {"vs_5_0", "hs_5_0", "ds_5_0", "gs_5_0", "ps_5_0", "cs_5_0"};

	D3D_SHADER_MACRO		macros[2];
	macros[0].Name			= "MATERIAL_FLAG";
	macros[0].Definition	= macroValue;
	macros[1].Name			= NULL;
	macros[1].Definition	= NULL;

	HRESULT hr				= S_OK;
	ID3DBlob* shaderCode	= NULL;
	ID3DBlob* compileMsg	= NULL;

	hr = compiler(mContent, mCodeSize, NULL, (CONST D3D_SHADER_MACRO*)macros, shaderInclude, "main", target[mShaderType], 
						0, 0, &shaderCode, &compileMsg);			
	if(hr == S_OK)
	{
		ID3D11Device* device = kiRenderer::getInstance()->getD3D11Device();
		void* d3d11Shader = NULL;
		hr = S_FALSE;
		///< dynamic linkage
		//ID3D11ClassLinkage* linkage = NULL;
		//device->CreateClassLinkage(&linkage);
		switch(mShaderType)
		{
		case kiShader::SD_VS:
			hr = device->CreateVertexShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL/*linkage*/, (ID3D11VertexShader**)(&d3d11Shader));
			break;
		case kiShader::SD_HS:
			hr = device->CreateHullShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL/*linkage*/, (ID3D11HullShader**)(&d3d11Shader));
			break;
		case kiShader::SD_DS:
			hr = device->CreateDomainShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL/*linkage*/, (ID3D11DomainShader**)(&d3d11Shader));
			break;
		case kiShader::SD_GS:
			hr = device->CreateGeometryShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL/*linkage*/, (ID3D11GeometryShader**)(&d3d11Shader));
			break;
		case kiShader::SD_PS:
			hr = device->CreatePixelShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL/*linkage*/, (ID3D11PixelShader**)(&d3d11Shader));
			break;
		case kiShader::SD_CS:
			hr = device->CreateComputeShader(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), NULL/*linkage*/, (ID3D11ComputeShader**)(&d3d11Shader));
			break;
		default:
			break;
		}

		if(hr == S_OK)
		{
			bytecode = shaderCode;
			return d3d11Shader;
		}

		bytecode = NULL;
		return NULL;
	}
	else
	{
		bytecode = NULL;
		char* error = (char*)compileMsg->GetBufferPointer();
		compileMsg->Release();
		return NULL;
	}
}
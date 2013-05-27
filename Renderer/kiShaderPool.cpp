#include "kiShaderPool.h"
#include "kiRenderer.h"

kiShaderInclude::kiShaderInclude(kiShaderPool* pool)
: mShaderPool(pool)
{
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE kiShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, \
																	 LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	if(mShaderPool == NULL)
		return FALSE;

	TCHAR* file = NULL;
#ifdef _UNICODE
	TCHAR buff[MAX_PATH];
	int wcount = MultiByteToWideChar(CP_ACP, 0, pFileName, (int)strlen(pFileName) + 1, buff, 0xff);
	file = buff;
#else
	file = pFileName;
#endif
	UINT shaderID = kiShaderPool::convShaderID(file);
	if(shaderID == INVALID_SHADER_ID)
		return S_FALSE;

	kiShader* shader = mShaderPool->getShader(shaderID);
	UINT codeSize = 0;
	VOID* code = shader->getCodeBuffer(codeSize);
	if(code == NULL || codeSize == 0)
		return S_FALSE;

	*ppData = code;
	*pBytes = codeSize;
	return S_OK;
}

COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE kiShaderInclude::Close(LPCVOID pData)
{
	return S_OK;
}

///<**********************************************************************************
kiShaderPool* kiShaderPool::ms_instance = NULL;
BOOL __stdcall shaderCallback(const TCHAR* path, BOOL isDir, void* data, UINT dataSize)
{
	BOOL result = FALSE;

	kiShaderPool* pool = (kiShaderPool*)data;
	if(pool != NULL && dataSize == sizeof(kiShaderPool*))
	{
		TCHAR temp[MAX_PATH];
		result = kiFileUtility::extractName(path, temp, MAX_PATH);
		if(!result)
			return TRUE;

		UINT shaderID = kiShaderPool::convShaderID(temp);
		if(shaderID == INVALID_SHADER_ID)
			return TRUE;

		result = kiFileUtility::extractExt(path, temp, MAX_PATH);
		if(!result)
			return TRUE;

		kiShader::eShaderType type = kiShaderPool::convShaderType(temp);
		if(type == kiShader::SD_INVALID)
			return TRUE;

		kiFile shaderFile;
		result = shaderFile.openFile(path, TEXT("rb"));
		if(!result)
			return TRUE;

		unsigned int size = shaderFile.getSize();
		unsigned char* buffer = (unsigned char*)kiAlloc1(size);
		shaderFile.read(buffer, size);
		shaderFile.closeFile();
		pool->appendShader(type, shaderID, buffer, size);
		return TRUE;
	}

	return FALSE;
}

kiShaderPool::kiShaderPool()
: mShaderCompiler(NULL)
, mShaderReflect(NULL)
, mShaderInclude(NULL)
, mCompilerModule(NULL)
{

}

kiShaderPool::~kiShaderPool()
{

}

UINT kiShaderPool::convShaderID(const TCHAR* name)
{
	UINT code = INVALID_SHADER_ID;
	UINT len = tstrlen(name);
	if(len > 4 && name[4] == TEXT('#'))
	{
		if( name[0] >= TEXT('0') && name[0] <= TEXT('9') && \
			name[1] >= TEXT('0') && name[1] <= TEXT('9') && \
			name[2] >= TEXT('0') && name[2] <= TEXT('9') && \
			name[3] >= TEXT('0') && name[3] <= TEXT('9'))
		{
			code = (((name[0] - TEXT('0')) << 24) & 0xff000000) | (((name[1] - TEXT('0')) << 16) & 0x00ff0000) | \
					(((name[2] - TEXT('0')) << 8) & 0x0000ff00) | (((name[3] - TEXT('0')) << 0) & 0x000000ff);
			return code;
		}
	}

	return code;
}

kiShader::eShaderType kiShaderPool::convShaderType(const TCHAR* ext)
{
	if(tstrcmp(ext, TEXT("vs11")) == 0)
		return kiShader::SD_VS;
	else if(tstrcmp(ext, TEXT("hs11")) == 0)
		return kiShader::SD_HS;
	else if(tstrcmp(ext, TEXT("ds11")) == 0)
		return kiShader::SD_DS;
	else if(tstrcmp(ext, TEXT("gs11")) == 0)
		return kiShader::SD_GS;
	else if(tstrcmp(ext, TEXT("ps11")) == 0)
		return kiShader::SD_PS;
	else if(tstrcmp(ext, TEXT("cs11")) == 0)
		return kiShader::SD_CS;
	else if(tstrcmp(ext, TEXT("h")) == 0)
		return kiShader::SD_SH;
	else
		return kiShader::SD_INVALID;
}

BOOL kiShaderPool::init()
{
	///< Éú³ÉshaderÄ¿Â¼
	TCHAR path[MAX_PATH];
	kiFileUtility::getBinDir(path, MAX_PATH);

	///< load d3dcompiler_43
	tstrcat(path, MAX_PATH, TEXT("D3DCompiler_46.dll"));
	mCompilerModule = LoadLibrary(path);
	mShaderInclude = kiNew1 kiShaderInclude(this);
	if(mCompilerModule != NULL)
	{
		mShaderCompiler = (shader_comiler)::GetProcAddress(mCompilerModule, "D3DCompile");
		mShaderReflect = (shader_reflect)::GetProcAddress(mCompilerModule, "D3DReflect");
		if(mShaderCompiler != NULL && mShaderReflect != NULL)
		{
			kiFileUtility::getRootDir(path, MAX_PATH);
			tstrcat(path, MAX_PATH, TEXT("shader\\"));

			kiFileUtility fu;
			UINT fileCount = fu.dirEnum(path, TRUE, shaderCallback, this, sizeof(kiShaderPool*), 0);

			//getShader(5)->getD3D11Shader(1);
			ms_instance = this;
			return TRUE;				
		}
	}

	finl();
	return FALSE;
}

void kiShaderPool::appendShader(kiShader::eShaderType shaderType, UINT shaderID, void* sourceCode, UINT size)
{
	map<UINT, kiShader*>::iterator iter = mShaderDB.find(shaderID);
	if(iter == mShaderDB.end())
	{
		kiShader* shader = kiNew1 kiShader(shaderType, sourceCode, size);
		BOOL result = shader->init();
		if(result)
		{
			mShaderDB[shaderID] = shader;
			return;
		}
	}
	
	__asm int 3;
}

VOID kiShaderPool::finl()
{
	ms_instance = NULL;

	mShaderCompiler = NULL;
	mShaderReflect = NULL;

	if(mShaderInclude != NULL)
	{
		kiDelete1 mShaderInclude;
		mShaderInclude = NULL;
	}

	map<UINT, kiShader*>::iterator iter = mShaderDB.begin();
	for ( ; iter != mShaderDB.end(); ++iter)
	{
		if(iter->second != NULL)
		{
			iter->second->finl();
			kiDelete1 iter->second;
		}
	}
	mShaderDB.clear();

	if(mCompilerModule != NULL)
		FreeLibrary(mCompilerModule);
}

kiShader* kiShaderPool::getShader(UINT shaderID)
{
	map<UINT, kiShader*>::iterator iter = mShaderDB.find(shaderID);
	if(iter == mShaderDB.end())
		return NULL;
	else
		return iter->second;
}
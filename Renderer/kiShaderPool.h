#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiShader.h"

class kiShader;
class kiShaderPool;

class kiShaderInclude : public kiMemObject, public ID3DInclude
{
public:
	kiShaderInclude(kiShaderPool* pool);

	virtual COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, \
		LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	virtual COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE Close(LPCVOID pData);
private:
	kiShaderPool* mShaderPool;
};

class kiShaderPool : public kiMemObject
{
public:
	kiShaderPool();
	~kiShaderPool();

	BOOL					init();
	VOID					finl();

	kiShader*				getShader(UINT shaderID);
	inline shader_comiler	getCompiler();
	inline shader_reflect	getReflect();
	inline kiShaderInclude*	getShaderInclude();

	///< internal use only
	void					 appendShader(kiShader::eShaderType shaderType, UINT shaderID, void* sourceCode, UINT size);
	static UINT				convShaderID(const TCHAR* fileName);
	static kiShader::eShaderType convShaderType(const TCHAR* ext);
protected:
	map<UINT, kiShader*>		mShaderDB;
	kiShaderInclude*			mShaderInclude;
	shader_comiler				mShaderCompiler;
	shader_reflect				mShaderReflect;
	HMODULE						mCompilerModule;

	static kiShaderPool*		ms_instance;
public:
	static kiShaderPool* getInstance()
	{
		return ms_instance;
	}
};

inline shader_comiler kiShaderPool::getCompiler()
{
	return mShaderCompiler;
}

inline shader_reflect kiShaderPool::getReflect()
{
	return mShaderReflect;
}

inline kiShaderInclude* kiShaderPool::getShaderInclude()
{
	return mShaderInclude;
}

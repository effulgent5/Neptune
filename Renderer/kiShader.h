#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"

#define INVALID_SHADER_ID		0xffffffff

typedef HRESULT (WINAPI *shader_comiler)(
	LPCVOID pSrcData,
	SIZE_T SrcDataSize,
	LPCSTR pSourceName,
	CONST D3D_SHADER_MACRO* pDefines,
	ID3DInclude* pInclude,
	LPCSTR pEntrypoint,
	LPCSTR pTarget,
	UINT Flags1,
	UINT Flags2,
	ID3DBlob** ppCode,
	ID3DBlob** ppErrorMsgs
	);

typedef HRESULT (WINAPI *shader_reflect)(
	LPCVOID pSrcData,
	SIZE_T SrcDataSize,
	REFIID pInterface,
	void** ppReflector);

class kiShader : public kiMemObject
{
public:
	enum eShaderType
	{
		SD_VS = 0,
		SD_HS = 1,
		SD_DS = 2,
		SD_GS = 3,
		SD_PS = 4,
		SD_CS = 5,
		SD_TOTAL = 6,
		SD_SH = 7,
		SD_INVALID = 8
	};

	kiShader(kiShader::eShaderType type, void* code, UINT codeSize);
	virtual ~kiShader();

	BOOL			init();
	VOID			finl();

	VOID*			getD3D11Shader(UINT matFlag = 0);
	ID3DBlob*		getBytecode(UINT matFlag = 0);
	eShaderType		getShaderType();

	inline VOID*	getCodeBuffer(UINT& bufferSize);
protected:
	VOID*			compilerShader(UINT matFlag, ID3DBlob*& bytecode);
protected:
	eShaderType		mShaderType;
	VOID*			mContent;
	UINT			mCodeSize;
	map<UINT, VOID*> mD3D11ShaderDB;
	map<UINT, ID3DBlob*> mBytecodeDB;
};

inline VOID* kiShader::getCodeBuffer(UINT& bufferSize)
{
	bufferSize = mCodeSize;
	return mContent;
}
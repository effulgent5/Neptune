#pragma once

typedef BOOL (__stdcall *EnuCallBack)(const TCHAR* path, BOOL isDir, void* data, UINT dataSize);

class kiFileUtility
{
public:
	static BOOL makeReadOnly(const TCHAR* fileName);
	static BOOL makeWriteable(const TCHAR* fileName);
	static BOOL stdPath(const TCHAR* src, TCHAR* dst);
	static BOOL extractPath(const TCHAR* src, TCHAR* path, UINT size);
	static BOOL extractExt(const TCHAR* src, TCHAR* path, UINT size);
	static BOOL extractName(const TCHAR* src, TCHAR* path, UINT size);
	static BOOL getRootDir(TCHAR* path, UINT size);
	static BOOL getBinDir(TCHAR* path, UINT size);

	UINT dirEnum(const TCHAR* path, BOOL enumSubDir, EnuCallBack callback, void* userData, UINT dataSize, int maxDepth);
protected:
	void dirFileEnum_r(TCHAR* path, int curDepth);

private:
	///< used by dirEnum
	int			mMaxDepth;
	EnuCallBack mCallback;
	void*		mUserData;
	UINT		mDataSize;
	BOOL		mEnumSubDir;
	int			mTotalFileNum;
	///< end
	///< temp var
};
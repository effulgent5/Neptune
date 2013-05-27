#pragma once
#include "systemCommon.h"
#include "commonDefines.h"
#include "Memory/kiMemObject.h"

class COMMON_EI kiMessageClient : public kiMemObject
{
public:
	kiMessageClient();
	virtual ~kiMessageClient();

	enum eMsgResult
	{
		MR_DLD_END = 0,	///< 消息已被处理,不可以继续向下传递
		MR_DLD_CTU = 1,	///< 消息已被处理,可以继续向下传递
		MR_PAS_END = 2,	///< 消息未处理,不可以继续向下传递
		MR_PAS_CTU = 3,	///< 消息未处理,可以继续向下传递
		MR_TOTAL = 4,
		MR_INVALID = 5
	};

	virtual eMsgResult onMsg(UINT msg, VOID* data);
};
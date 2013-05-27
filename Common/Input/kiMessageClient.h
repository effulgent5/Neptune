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
		MR_DLD_END = 0,	///< ��Ϣ�ѱ�����,�����Լ������´���
		MR_DLD_CTU = 1,	///< ��Ϣ�ѱ�����,���Լ������´���
		MR_PAS_END = 2,	///< ��Ϣδ����,�����Լ������´���
		MR_PAS_CTU = 3,	///< ��Ϣδ����,���Լ������´���
		MR_TOTAL = 4,
		MR_INVALID = 5
	};

	virtual eMsgResult onMsg(UINT msg, VOID* data);
};
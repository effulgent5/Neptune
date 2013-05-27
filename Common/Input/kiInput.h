#pragma once
#include "systemCommon.h"
#include "commonDefines.h"
#include "Memory/kiMemObject.h"
#include "kiMessageDefines.h"
#include "kiInputDefines.h"

class kiMessageClient;

class COMMON_EI kiInput : public kiMemObject
{
public:
	kiInput();
	virtual ~kiInput();

	struct stInputInfo
	{
		INT input;
		USHORT flag;		///< 8bit unused - 4bit: key property - 4bit: time property
		USHORT interval;
	};

	BOOL init();
	VOID finl();

	BOOL regClient(kiMessageClient* client);
	BOOL regMsg(UINT msg, stInputInfo* info, UINT count, VOID* data);

	VOID tick();
};
#pragma once
#include "iExporter.h"

class ssExporter : public iExporter
{
public:
	ssExporter();
	virtual ~ssExporter();

	virtual BOOL exportObj(const TCHAR* fileName);
};
#pragma once
#include "commonExporter.h"

class iExporter
{
public:
	virtual BOOL exportObj(const TCHAR* fileName) = 0;
};
#pragma once
#include "IRenderParam.h"

class IImage;
class IBanal : public IRenderParam
{
public:
	virtual ~IBanal(){}

	virtual IImage* KICALL_TYPE	getBaseTex() const					= 0;
	virtual INT KICALL_TYPE		setBaseTex(IImage* image)			= 0;
	virtual UINT KICALL_TYPE	getSpcPower() const					= 0;
	virtual void KICALL_TYPE	setSpcPower(UINT power)				= 0;
	virtual const kVec3& KICALL_TYPE getColorBias() const				= 0;
	virtual void KICALL_TYPE	setColorBias(const kVec3& color)	= 0;

	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE	Dummy_getTypeClass()				= 0;
};
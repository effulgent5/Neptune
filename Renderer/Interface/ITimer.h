#include "IRenderBase.h"

class ITimer : public IRenderBase
{
public:
	virtual ~ITimer(){}
	///< �����к����õ��麯��
	virtual VOID KICALL_TYPE	Dummy_getTypeClass()				= 0;
};
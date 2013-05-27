#include "IRenderBase.h"

class ITimer : public IRenderBase
{
public:
	virtual ~ITimer(){}
	///< 用来中和无用的虚函数
	virtual VOID KICALL_TYPE	Dummy_getTypeClass()				= 0;
};
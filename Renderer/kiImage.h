#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderBase.h"

class kiImage : public kiRenderBase
{
protected:
	kiImage();
public:	
	virtual ~kiImage();

	///< 必须放在所有接口之后
	_DECLARE_DYNAMIC_(kiImage)
};
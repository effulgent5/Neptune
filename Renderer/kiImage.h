#include "commonCommon.h"
#include "rendererDefines.h"
#include "kiRenderBase.h"

class kiImage : public kiRenderBase
{
protected:
	kiImage();
public:	
	virtual ~kiImage();

	///< ����������нӿ�֮��
	_DECLARE_DYNAMIC_(kiImage)
};
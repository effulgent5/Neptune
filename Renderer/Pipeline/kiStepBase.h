#pragma once
#include "commonCommon.h"
#include "rendererDefines.h"

class kiSolid;
class kiTransform;
class kiRenderParam;

class kiStepBase : public kiMemObject
{
public:
	kiStepBase(UINT ID);
	virtual ~kiStepBase();

	struct stRenderObj
	{
		stRenderObj(kiSolid* _s, kiTransform* _w, kiRenderParam* _p)
			: s(_s), w(_w), p(_p)
		{

		}
		kiSolid* s;
		kiTransform* w;
		kiRenderParam* p;
	};

	virtual BOOL init();
	virtual VOID finl();

	virtual VOID clear();
	virtual VOID draw();
	virtual VOID insert(kiSolid* s, kiTransform* w, kiRenderParam* p);
	virtual VOID updatePerStepCB();


protected:
	UINT mStepID;
	vector<stRenderObj> mRenderSequence;
};
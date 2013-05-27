#include "kiBanal.h"
#include "kiShader.h"
#include "kiRenderer.h"

IMPL_DYNAMIC(kiBanal, kiRenderParam)

kiBanal::kiBanal()
{
	mColorBias = kVec3::UNITALL;
	mSpcPower = 0;
	mBaseTex = NULL;
}

kiBanal::~kiBanal()
{

}

BOOL kiBanal::init(VOID* initData, UINT size)
{
	kiRenderer* renderer = kiRenderer::getInstance();
	if(renderer == NULL)
		return FALSE;

	mMaterial = renderer->getMaterial(kiMaterial::MT_BANL);
	if(mMaterial != NULL)
		return TRUE;
	else
		return FALSE;
}

VOID kiBanal::finl()
{

}

IImage* kiBanal::getBaseTex() const
{
	return NULL;
	//return mBaseTexName;
}

VOID kiBanal::setBaseTex(IImage* name)
{

}

UINT kiBanal::getSpcPower() const
{
	return 0;
}

VOID kiBanal::setSpcPower(UINT power)
{

}

const kVec3& kiBanal::getColorBias() const
{
	return mColorBias;
}

VOID kiBanal::setColorBias(const kVec3& color)
{

}
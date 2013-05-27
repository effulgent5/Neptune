#pragma once
#include "systemCommon.h"
#include "commonDefines.h"

class kMesh;
class kRay;
class kiCamera;

class COMMON_EI kUtilities
{
public:
	kUtilities();
	~kUtilities();

	static kMesh* createUnitSphere(unsigned int longitude, unsigned int latitude);
	static kMesh* createBox(float x, float y, float z);
	static kMesh* createCapsule();
	static kMesh* createTube();
	static kMesh* createColumn();
	static kMesh* createPrism();

	static unsigned short float32ToFloat16(float f);

	//static void mouseToRay(float mouseX, float mouseY, float screenW, float screenH, const kiCamera& camera,  kRay& ray);
};
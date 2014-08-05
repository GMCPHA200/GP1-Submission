#ifndef _cCOLLECTABLE_H
#define _cCOLLECTABLE_H
#include "cSprite.h"
#include "cD3DManager.h"

class cCollectable : public cSprite
{
private:		
	//Transformation vector
	D3DXVECTOR2 collectableTrans;
	//Velocities vector
	D3DXVECTOR2 collectableVeloc;
	//Rotation angle
	float collectableRot;
	float collectableRotSpeed;
	//Maximum velocity limit
	D3DXVECTOR2 collectableMaxVeloc;
public:
	cCollectable();// Default constructor
	cCollectable(cD3DManager* d3dMgr, D3DXVECTOR2 startPos, D3DXVECTOR2 startVeloc, float startRot, D3DXVECTOR2 maxVeloc); // Constructor
	~cCollectable();// Destructor		
	void setCollectableTrans(D3DXVECTOR2 trans);
	void setCollectableVeloc(D3DXVECTOR2 veloc);		
	D3DXVECTOR2 getCollectableMaxVeloc();
	void update(float deltaTime);
};
#endif
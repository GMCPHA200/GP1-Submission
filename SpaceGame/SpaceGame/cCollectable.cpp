#include "cCollectable.h"

cCollectable:: cCollectable()//Default Constructor
{
}
cCollectable:: cCollectable(cD3DManager* d3dMgr, D3DXVECTOR2 startPos, D3DXVECTOR2 startVeloc, float rotSpeed, D3DXVECTOR2 maxVeloc) : cSprite(D3DXVECTOR3(startPos.x,startPos.y,0),d3dMgr->getTheD3DDevice(),"Images\\spaceman.png") // Constructor
{
	collectableTrans = startPos;
	collectableVeloc = startVeloc;
	collectableRot = 0;
	collectableRotSpeed = rotSpeed;
	collectableMaxVeloc = maxVeloc;
	cSprite::setSpriteScaling(0.5,0.5);
}
cCollectable::~cCollectable()//Destructor
{
}

void cCollectable::setCollectableTrans(D3DXVECTOR2 trans)
{
	collectableTrans = trans;
}
void cCollectable::setCollectableVeloc(D3DXVECTOR2 veloc)
{
	collectableVeloc = veloc;
}
D3DXVECTOR2 cCollectable::getCollectableMaxVeloc()
{
	return collectableMaxVeloc;
}

void cCollectable::update(float deltaTime)
{
	collectableTrans += collectableVeloc * deltaTime;
	collectableRot += collectableRotSpeed;

	if(collectableVeloc.x > collectableMaxVeloc.x)
	{
		collectableVeloc.x = collectableMaxVeloc.x;
	}
	if(collectableVeloc.x < -collectableMaxVeloc.x)
	{
		collectableVeloc.x = -collectableMaxVeloc.x;
	}
	if(collectableVeloc.y > collectableMaxVeloc.y)
	{
		collectableVeloc.y = collectableMaxVeloc.y;	
	}
			
	
	cCollectable::setSpritePos(D3DXVECTOR3(collectableTrans.x,collectableTrans.y,0.0f));
	cCollectable::setSpriteRotation(collectableRot);
	cSprite::update();
}
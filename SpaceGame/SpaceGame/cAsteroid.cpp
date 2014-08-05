#include "cAsteroid.h"

cAsteroid::cAsteroid() // Default constructor
{
}

cAsteroid::cAsteroid(cD3DManager* d3dMgr, D3DXVECTOR2 startPos, D3DXVECTOR2 startVeloc, float rotSpeed, D3DXVECTOR2 maxVeloc) : cSprite(D3DXVECTOR3(startPos.x,startPos.y,0),d3dMgr->getTheD3DDevice(),"Images\\asteroid.png") // Constructor
{	
	asteroidTrans = startPos;
	asteroidVeloc = startVeloc;
	asteroidRot = 0;
	asteroidRotSpeed = rotSpeed;
	asteroidMaxVeloc = maxVeloc;
}

cAsteroid::~cAsteroid()			// Destructor
{
}

void cAsteroid::setAsteroidTrans(D3DXVECTOR2 trans)
{
	asteroidTrans = trans;
}

void cAsteroid::setAsteroidVeloc(D3DXVECTOR2 veloc)
{
	asteroidVeloc = veloc;
}

void cAsteroid::setAsteroidRotSpeed(float rot)
{
	asteroidRotSpeed = rot;
}

float cAsteroid::getAsteroidRotSpeed()
{
	return asteroidRotSpeed;
}

D3DXVECTOR2 cAsteroid::getAsteroidVeloc()
{
	return asteroidVeloc;
}

D3DXVECTOR2 cAsteroid::getAsteroidMaxVeloc()
{
	return asteroidMaxVeloc;
}


void cAsteroid::update(float deltaTime)
{		
	asteroidTrans += asteroidVeloc * deltaTime;
	asteroidRot += asteroidRotSpeed;

	if(asteroidVeloc.x > asteroidMaxVeloc.x)
	{
		asteroidVeloc.x = asteroidMaxVeloc.x;
	}
	if(asteroidVeloc.x < -asteroidMaxVeloc.x)
	{
		asteroidVeloc.x = -asteroidMaxVeloc.x;
	}
	if(asteroidVeloc.y > asteroidMaxVeloc.y)
	{
		asteroidVeloc.y = asteroidMaxVeloc.y;	
	}
			
	
	cAsteroid::setSpritePos(D3DXVECTOR3(asteroidTrans.x,asteroidTrans.y,0.0f));
	cAsteroid::setSpriteRotation(asteroidRot);
	cSprite::update();			
}
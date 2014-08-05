#ifndef _cASTEROID_H
#define _cASTEROID_H
#include "cSprite.h"
#include "cD3DManager.h"

class cAsteroid : public cSprite
{
private:		
	//Transformation vector
	D3DXVECTOR2 asteroidTrans;
	//Velocities vector
	D3DXVECTOR2 asteroidVeloc;
	//Rotation angle
	float asteroidRot;
	float asteroidRotSpeed;
	//Maximum velocity limit
	D3DXVECTOR2 asteroidMaxVeloc;
public:
	cAsteroid();// Default constructor
	cAsteroid(cD3DManager* d3dMgr, D3DXVECTOR2 startPos, D3DXVECTOR2 startVeloc, float startRot, D3DXVECTOR2 maxVeloc); // Constructor
	~cAsteroid();// Destructor		
	void setAsteroidTrans(D3DXVECTOR2 trans);
	void setAsteroidVeloc(D3DXVECTOR2 veloc);
	void setAsteroidRotSpeed(float rot);
	float getAsteroidRotSpeed();
	D3DXVECTOR2 getAsteroidVeloc();	
	D3DXVECTOR2 getAsteroidMaxVeloc();
	void update(float deltaTime);
};
#endif
#ifndef _CPLAYER_H
#define _CPLAYER_H
#include "cSprite.h"
#include "cD3DManager.h"

class cPlayer : public cSprite
{
private:		
	//Transformation vector
	D3DXVECTOR2 shipTrans;
	//Velocities vector
	D3DXVECTOR2 shipVeloc;
	//Rotation angle
	float shipRot;
	//Maximum velocity limit
	D3DXVECTOR2 shipMaxVeloc;
	//Maximum acceleration limit
	D3DXVECTOR2 shipMaxAccel;
	//Larger maximum forward velocity trigger
	bool increasedMaxVeloc;
	//Frame counter for sprite animation
	int counter;
	//Toggle for sprite animation;
	bool animationToggle;
	//Texture array
	cD3DXTexture* (*textures)[6];
	//Player health
	int health;
	//Invincibility toggle to avoid instantly draining all health at once
	bool invincible;
	//Counter to make the sprite flash when invincible
	int invincibleFlashCounter;
	//Counter to turn off invincibility after a time has passed
	int invincibleCounter;
	//Toggle for making the sprite flash
	bool invincibleAnimationToggle;
	//The score for collecting the collectables
	int score;
	
public:
	cPlayer();// Default constructor
	cPlayer(cD3DManager* d3dMgr, D3DXVECTOR2 startPos, D3DXVECTOR2 startVeloc, float startRot, D3DXVECTOR2 maxVeloc, D3DXVECTOR2 maxAccel, cD3DXTexture* (*playerTextures)[6]); // Constructor
	~cPlayer();// Destructor		
	int getHealth();
	int getScore();
	void increaseScore();
	void setShipTrans(D3DXVECTOR2 trans);
	void hit();
	void update(bool spaceButton, bool upButton, bool rightButton, bool leftButton, bool downButton, float deltaTime);
};
#endif
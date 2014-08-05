#include "cPlayer.h"

cPlayer::cPlayer() // Default constructor
{
}

cPlayer::cPlayer(cD3DManager* d3dMgr, D3DXVECTOR2 startPos, D3DXVECTOR2 startVeloc, float startRot, D3DXVECTOR2 maxVeloc, D3DXVECTOR2 maxAccel, cD3DXTexture* (*playerTextures)[6]) : cSprite(D3DXVECTOR3(startPos.x,startPos.y,0), d3dMgr->getTheD3DDevice(),(*playerTextures)[0]) // Constructor
{	
	shipTrans = startPos;
	shipVeloc = startVeloc;
	shipRot = startRot;
	shipMaxVeloc = maxVeloc;
	shipMaxAccel = maxAccel;
	textures = playerTextures;
	counter = 0;
	animationToggle = false;
	health = 5;
	invincible = false;
	invincibleCounter = 0;
	invincibleFlashCounter = 0;
	invincibleAnimationToggle = false;
	score = 0;
}

cPlayer::~cPlayer()			// Destructor
{
}

int cPlayer::getHealth()
{
	return health;
}

int cPlayer::getScore()
{
	return score;
}

void cPlayer::increaseScore()
{
	score++;
}

void cPlayer::hit()
{
	if (invincible == false)
	{
		health--;
	}
	invincible = true;
}

void cPlayer::setShipTrans(D3DXVECTOR2 trans)
{
	shipTrans = trans;
}


void cPlayer::update(bool spaceButton, bool upButton, bool rightButton, bool leftButton, bool downButton, float deltaTime)
{
	//Reset sprite texture to default
	if(invincibleAnimationToggle == false)
	{
		cPlayer::setTexture((*textures)[0]);
	}
	else
	{
		cPlayer::setTexture((*textures)[5]);
	}
	//Add to frame counter
	counter++;
	//If on or after 5th frame...
	if (counter >= 5)
	{
		//Reset frame counter and switch sprite animation toggle's value
		counter = 0;
		animationToggle = !animationToggle;
	}

	if(invincible == true)
	{
		invincibleFlashCounter++;
		invincibleCounter++;
			//If on or after 15th frame...
			if (invincibleFlashCounter >= 15)
			{
				//Reset frame counter and switch sprite animation toggle's value
				invincibleFlashCounter = 0;				
				invincibleAnimationToggle = !invincibleAnimationToggle;				
				//If after one second of total invincibility
				if(invincibleCounter >= 60)
				{
					//Set not invincible
					invincibleCounter=0;
					invincible = false;
				}
			}
	}
	
			
	increasedMaxVeloc = false;

	if (spaceButton)
	{
		//Fires reactionary thrusters to bring ship to full stop

		//If statements which reduce all velocities 
		//First checks if current velocity is close to zero and then sets it to zero, to avoid passing zero and increasing velocity in the opposite direction
		if(shipVeloc.x > -(shipMaxAccel.x * 2) && shipVeloc.x < (shipMaxAccel.x * 2))
		{
			shipVeloc.x = 0.0f;
		}
		if(shipVeloc.y > -(shipMaxAccel.y * 2) && shipVeloc.y < (shipMaxAccel.x * 2))
		{
			shipVeloc.y = 0.0f;
		}
				
		if(shipVeloc != D3DXVECTOR2(0.0f, 0.0f))
		{
			//These if statements check whether the velocity is positive or negative, and then counteracts the force
			if(shipVeloc.x > 0)
			{
				shipVeloc.x -= shipMaxAccel.x * 2; 
			}
			if(shipVeloc.y > 0)
			{
				shipVeloc.y -= shipMaxAccel.y * 2;
			}
			if(shipVeloc.x < 0)
			{
				shipVeloc.x += shipMaxAccel.x * 2;
			}
			if(shipVeloc.y < 0)
			{
				shipVeloc.y += shipMaxAccel.y * 2;
			}
		}	
				
	}
	else
	{
		//Disables higher max velocity in case the up button is no longer held
		increasedMaxVeloc = false;

   		if (upButton || (leftButton && rightButton))
		{
			//Fire both engines, moving the ship forward and resulting in higher top speed. Also gradually stops rotation

			//If not becoming invisible due to invincibility...
			if(invincibleAnimationToggle == false)
			{
				//If toggle is true...
				if(animationToggle == true)
				{
					//Set texture to first of animation
					cPlayer::setTexture((*textures)[3]);				
				}
				else
				{
					//If not true set texture to second of animation
					cPlayer::setTexture((*textures)[4]);				
				}
			}
			else // Else set texture to the invisible texture;
			{
				cPlayer::setTexture((*textures)[5]);
			}

			//Allow higher max velocity for forward direction
			increasedMaxVeloc = true;
			//Increase forward velocity
			shipVeloc.y -= shipMaxAccel.y;

			//If statements which reduce rotation velocities due to rapid forward acceleration
			//First checks if current velocity is close to zero and then sets it to zero, to avoid passing zero and increasing velocity in the opposite direction
			if(shipVeloc.x > -(shipMaxAccel.x * 2) && shipVeloc.x < (shipMaxAccel.x * 2))
			{
				shipVeloc.x = 0.0f;
			}
					
			if(shipVeloc.x != 0.0f)
			{
				//These if statements check whether the velocity is positive or negative, and then counteracts the force


				if(shipVeloc.x > 0)
				{					
					shipVeloc.x -= shipMaxAccel.x;
				}
				if(shipVeloc.x < 0)
				{					
					shipVeloc.x += shipMaxAccel.x;
				}		
			}
		}

		if (leftButton && !rightButton && !downButton && !upButton)
		{
			//Fire right engine, rotating ship anti-clockwise and increasing forward velocity slightly
			
			//If not becoming invisible due to invincibility...
			if(invincibleAnimationToggle == false)
			{
				//If toggle is true...
				if(animationToggle == true)
				{
					//Set texture to first of animation
					cPlayer::setTexture((*textures)[1]);
					cPlayer::setSpriteScaling(-1.0,1.0);
				}
				else
				{
					//If not true set texture to second of animation
					cPlayer::setTexture((*textures)[2]);
					cPlayer::setSpriteScaling(-1.0,1.0);
				}
			}
			else // Else set texture to the invisible texture;
			{
				cPlayer::setTexture((*textures)[5]);
			}

			//If moving forwards...
			if(shipVeloc.y < 0)
			{
				//Increase anti-clockwise rotation velocity  
				shipVeloc.x -= shipMaxAccel.x;
				//Increases forward velocity
				shipVeloc.y -= shipMaxAccel.y / 5;
			}
			else //Fast forward acceleration to counteract the reverse momentum 
			{
				shipVeloc.y -= shipMaxAccel.y;
			}
		}		

		if (rightButton && !leftButton && !downButton && !upButton)
		{
			//Fire left engine, rotating ship clockwise and increasing forward velocity slightly

			//If not becoming invisible due to invincibility...
			if(invincibleAnimationToggle == false)
			{
				//If toggle is true...
				if(animationToggle == true)
				{
					//Set texture to first of animation
					cPlayer::setTexture((*textures)[1]);
					cPlayer::setSpriteScaling(1.0,1.0);
				}
				else
				{
					//If not true set texture to second of animation
					cPlayer::setTexture((*textures)[2]);
					cPlayer::setSpriteScaling(1.0,1.0);
				}
			}
			else // Else set texture to the invisible texture;
			{
				cPlayer::setTexture((*textures)[5]);
			}

			//If moving forwards...
			if(shipVeloc.y < 0)
			{
				//Increase clockwise rotation velocity 
				shipVeloc.x += shipMaxAccel.x;
				//Increases forward velocity
				shipVeloc.y -= shipMaxAccel.y / 5;
			}
			else  //Fast forward acceleration to counteract the reverse momentum
			{						
				shipVeloc.y -= shipMaxAccel.y;
			}
		}

		if (downButton)
		{
			//Fire reverse thrusters, decreasing forward velocity and slowly stops rotation 

			//Increase reverse velocity
			shipVeloc.y += (shipMaxAccel.y / 3) * 2;

			//If statements which reduce rotation velocities
			//First checks if current velocity is close to zero and then sets it to zero, to avoid passing zero and increasing velocity in the opposite direction
			if(shipVeloc.x > -(shipMaxAccel.x) && shipVeloc.x < (shipMaxAccel.x))
			{
				shipVeloc.x = 0.0f;
			}
					
			if(shipVeloc.x != 0.0f)
				{
				//These if statements check whether the velocity is positive or negative, and then counteracts the force
				if(shipVeloc.x > 0)
				{
					shipVeloc.x -= shipMaxAccel.x / 2;
				}
				if(shipVeloc.x < 0)
				{
					shipVeloc.x += shipMaxAccel.x / 2;
				}
			}
		}
	}

	//Statements to stop ship from exceeding maximum velocities
	if(shipVeloc.x != 0.0f)
	{
		//These if statements check whether the velocity is positive or negative, and then counteracts the force
		if(shipVeloc.x > 0)
		{
			shipVeloc.x -= shipMaxAccel.x / 4;
		}
		if(shipVeloc.x < 0)
		{
			shipVeloc.x += shipMaxAccel.x / 4;
		}
	}
	if(shipVeloc.x > shipMaxVeloc.x)
	{
		shipVeloc.x = shipMaxVeloc.x;
	}
	if(shipVeloc.x < -shipMaxVeloc.x)
	{
		shipVeloc.x = -shipMaxVeloc.x;
	}
	if(shipVeloc.y > shipMaxVeloc.y)
	{
		shipVeloc.y = shipMaxVeloc.y;
	}
	//If ship is firing both engines then allows higher maximum forward velocity
	if(increasedMaxVeloc == true)
	{
		if(shipVeloc.y < -(shipMaxVeloc.y / 3) * 10)
		{
			shipVeloc.y = -(shipMaxVeloc.y / 3) * 10;
		}
	} //Else use normal maximum, but slowly deccelerate instead of instantly setting velocity to max value
	else if(shipVeloc.y < -shipMaxVeloc.y)
	{
		shipVeloc.y += shipMaxAccel.y*2;
	}	

	//Check to keep rotation inertia working properly in reverse, keeping the rotation consistent instead of switching direction like a car reversing
	if(shipVeloc.y <= 0) //Going forwards
	{
		//Update rotation
		shipRot += shipVeloc.x * (D3DX_PI/128);
	}
	if(shipVeloc.y > 0)//Going backwards
	{
		shipRot += -shipVeloc.x * (D3DX_PI/128);
	}

	//Calculate translation direction vector
	shipTrans.x += (shipVeloc.y * sin(-shipRot)) * deltaTime;
	shipTrans.y += (shipVeloc.y  * cos(-shipRot)) * deltaTime;	
			
	//Calculate transformation matrix
	cPlayer::setSpriteRotation((shipRot*180)/ D3DX_PI);
	cPlayer::setSpritePos(D3DXVECTOR3(shipTrans.x,shipTrans.y,0.0f));
	cSprite::update();
	
	//D3DXMatrixTransformation2D(&transformMatrix, NULL, 0.0f, NULL, &(getSpriteCentre()), shipRot, &shipTrans);		
}
#include "cButton.h"

cButton::cButton()
{
}
cButton::cButton(cD3DManager* d3dMgr, D3DXVECTOR2 startPos) : cSprite(D3DXVECTOR3(startPos.x,startPos.y,0),d3dMgr->getTheD3DDevice(),"Images\\button.png")// Default constructor
{
	pushed = false;	
	cSprite::setSpriteScaling(4,1);
	cSprite::update();
}

cButton::~cButton()
{
}

void cButton::hit()
{
	pushed = !pushed;
}

bool cButton::getHit()
{
	return pushed;
}
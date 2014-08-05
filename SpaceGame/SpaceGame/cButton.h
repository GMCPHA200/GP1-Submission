#ifndef _cBUTTON_H
#define _cBUTTON_H
#include "cSprite.h"
#include "cD3DManager.h"

class cButton : public cSprite
{
private:		
	bool pushed;
public:
	cButton();
	cButton(cD3DManager* d3dMgr, D3DXVECTOR2 startPos);// constructor	
	~cButton();// Destructor
	void hit();
	bool getHit();	
};
#endif
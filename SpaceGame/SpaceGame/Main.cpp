/*
=================
main.cpp
Main entry point for the Card application
=================
*/

#include "GameConstants.h"
#include "GameResources.h"
#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cD3DXTexture.h"
#include "cSprite.h"
#include "cXAudio.h"
#include "cD3DXFont.h"
#include "cCollectable.h"
#include "cAsteroid.h"
#include "cPlayer.h"
#include "cScoreboard.h"
#include "cButton.h"

using namespace std;

HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle

// Get a reference to the DirectX Manager
static cD3DManager* d3dMgr = cD3DManager::getInstance();

// Get a reference to the DirectX Sprite renderer Manager 
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();

//Booleans representing whether key is pressed or not
//Used to bypass the windows key repeat delay from messages to give more responsive input
bool leftButton = false;
bool rightButton = false;
bool upButton = false;
bool downButton = false;
bool spaceButton = false;

//int numButtons = 1;
//cButton* buttons[];

POINT mouseXY;

bool paused = false;

RECT clientBounds;

//cXAudio gExplodeSound;


/*
==================================================================
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
* LPARAM lParam)
* The window procedure
==================================================================
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messages from the queue
	switch (message)
	{
		case WM_KEYDOWN:
			{	
				if (wParam == VK_SPACE)
				{
					spaceButton = true;				
					return 0;
				}
				if (wParam == VK_LEFT)
				{
					leftButton = true;				
					return 0;
				}
				if (wParam == VK_UP)
				{
					upButton = true;						
					return 0;
				}
				if (wParam == VK_RIGHT)
				{
					rightButton = true;								
					return 0;
				}
				if (wParam == VK_DOWN)
				{
					downButton = true;						
					return 0;
				}	
				if(wParam == VK_ESCAPE)
				{
					paused = !paused;
				}
				return 0;				
			}
		case WM_KEYUP:
			{
				if (wParam == VK_SPACE)
				{
					spaceButton = false;				
					return 0;
				}
				if (wParam == VK_LEFT)
				{
					leftButton = false;				
					return 0;
				}
				if (wParam == VK_UP)
				{
					upButton = false;						
					return 0;
				}
				if (wParam == VK_RIGHT)
				{
					rightButton = false;								
					return 0;
				}
				if (wParam == VK_DOWN)
				{
					downButton = false;						
					return 0;
				}	
				return 0;
			}
		case WM_LBUTTONDOWN:
			{				
				mouseXY.x = LOWORD(lParam);
				mouseXY.y = HIWORD(lParam);								
				return 0;
			}
		case WM_CLOSE:
			{
			// Exit the Game
				PostQuitMessage(0);
				 return 0;
			}

		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
==================================================================
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window class for the application, creates the window
==================================================================
*/
bool initWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	// Fill in the WNDCLASSEX structure. This describes how the window
	// will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MyWindowIcon)); // icon to associate with the application
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);// the default cursor
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = "SpaceRescue"; // the class name being created
	wcex.hIconSm = LoadIcon(hInstance,"rocket.ico"); // the handle to the small icon

	RegisterClassEx(&wcex);
	// Create the window
	wndHandle = CreateWindow("SpaceRescue",			// the window class to use
							 "Space Rescue",			// the title bar text
							WS_OVERLAPPEDWINDOW,	// the window style
							CW_USEDEFAULT, // the starting x coordinate
							CW_USEDEFAULT, // the starting y coordinate
							800, // the pixel width of the window
							600, // the pixel height of the window
							NULL, // the parent window; NULL for desktop
							NULL, // the menu for the application; NULL for none
							hInstance, // the handle to the application instance
							NULL); // no values passed to the window
	// Make sure that the window handle that is created is valid
	if (!wndHandle)
		return false;
	// Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	return true;
}

/*
==================================================================
// This is winmain, the main entry point for Windows applications
==================================================================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Initialize the window
	if ( !initWindow( hInstance ) )
		return false;
	// called after creating the window
	if ( !d3dMgr->initD3DManager(wndHandle) )
		return false;
	if ( !d3dxSRMgr->initD3DXSpriteMgr(d3dMgr->getTheD3DDevice()))
		return false;

	// Grab the frequency of the high def timer
	__int64 freq = 0;				// measured in counts per second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	float sPC = 1.0f / (float)freq;			// number of seconds per count

	__int64 currentTime = 0;				// current time measured in counts per second;
	__int64 previousTime = 0;				// previous time measured in counts per second;

	float numFrames   = 0.0f;				// Used to hold the number of frames
	float timeElapsed = 0.0f;				// cumulative elapsed time

	GetClientRect(wndHandle,&clientBounds);

	float fpsRate = 1.0f/60.0f;	

	LPDIRECT3DSURFACE9 aSurface;				// the Direct3D surface
	LPDIRECT3DSURFACE9 theBackbuffer = NULL;  // This will hold the back buffer
	
	//Initialise the scoreboard
	cScore score(4,"Jeff");
	cScoreboard scoreboard(&score);
	cScore score2(6,"Dan");
	scoreboard.insertScore(&score2);
	cScore score3(2,"Drew");
	scoreboard.insertScore(&score3);
	cScore score4(0,"Brad");
	scoreboard.insertScore(&score4);
	
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	
	// load custom font
	cD3DXFont* customFont = new cD3DXFont(d3dMgr->getTheD3DDevice(),hInstance, "Cinema Gothic BTN Inline");


	//Create player object
	cD3DXTexture* playerTextures[6];
	char* playerTextureNames[] = {"Images\\rocketDefault.png","Images\\rocketOneSmall.png","Images\\rocketOneBig.png","Images\\rocketBothSmall.png", "Images\\rocketBothBig.png", "Images\\rocketInvisible.png"};	
	for (int i = 0; i < 6; i++)
	{
		playerTextures[i] = new cD3DXTexture(d3dMgr->getTheD3DDevice(), playerTextureNames[i]);
	}
	cPlayer* player = new cPlayer(d3dMgr, D3DXVECTOR2(clientBounds.right/2,clientBounds.bottom/2), D3DXVECTOR2(0,0), 0.0f, D3DXVECTOR2 (3.5f,150.0f), D3DXVECTOR2 (0.5f,5.0f), &playerTextures);
	
	//Create asteroid objects
	int const numAsteroids = 5;
	cAsteroid* asteroidArray[numAsteroids];
	srand ( (unsigned int)time(NULL) );
	for(int i = 0; i < numAsteroids; i++)
	{
		float asteroidRot = (-1) + static_cast <float>(rand()) / (static_cast <float> (RAND_MAX/ 2));
		asteroidArray[i] = new cAsteroid(d3dMgr, D3DXVECTOR2(-100,-300), D3DXVECTOR2(0.0f,0.0f), asteroidRot, D3DXVECTOR2(100.f,100.f));
		// Generate random variables, and a random spawn position for the asteroid		
		int asteroidCase = rand()%4 + 1;		
		float asteroidVelocX = (-asteroidArray[i]->getAsteroidMaxVeloc().x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(asteroidArray[i]->getAsteroidMaxVeloc().x)*2));
		float asteroidVelocY = (-asteroidArray[i]->getAsteroidMaxVeloc().y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(asteroidArray[i]->getAsteroidMaxVeloc().y)*2));
		float asteroidTransX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.right));
		float asteroidTransY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.bottom));
		  
		switch(asteroidCase)
		{
			case 1:	//Spawn top		
				asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(asteroidTransX, 0.0f));
				asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidVelocX,asteroidArray[i]->getAsteroidMaxVeloc().y));				
				break;

			case 2:	//Spawn right		
				asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(clientBounds.right, asteroidTransY));
				asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(-asteroidArray[i]->getAsteroidMaxVeloc().x, asteroidVelocY));				
				break;

			case 3:	//Spawn bottom		
				asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(asteroidTransX, clientBounds.bottom));
				asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidVelocX,-asteroidArray[i]->getAsteroidMaxVeloc().y));				
				break;

			case 4:	//Spawn left		
				asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(0.0f, asteroidTransY));
				asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidArray[i]->getAsteroidMaxVeloc().x,asteroidVelocY));				
				break;			
		}		
		
	}	

	//Create collectable object
	float collectableRot = (-1) + static_cast <float>(rand()) / (static_cast <float> (RAND_MAX/ 2));
	cCollectable* collectable = new cCollectable(d3dMgr, D3DXVECTOR2(-100,-300), D3DXVECTOR2(0.0f,0.0f), collectableRot, D3DXVECTOR2(50.f,50.f));
	// Generate random variables, and a random spawn position for the collectable		
	int collectableCase = rand()%4 + 1;		
	float collectableVelocX = (-collectable->getCollectableMaxVeloc().x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(collectable->getCollectableMaxVeloc().x)*2));
	float collectableVelocY = (-collectable->getCollectableMaxVeloc().y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(collectable->getCollectableMaxVeloc().y)*2));
	float collectableTransX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.right));
	float collectableTransY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.bottom));
		  
	switch(collectableCase)
	{
		case 1:	//Spawn top		
			collectable->setCollectableTrans(D3DXVECTOR2(collectableTransX, 0.0f));
			collectable->setCollectableVeloc(D3DXVECTOR2(collectableVelocX,collectable->getCollectableMaxVeloc().y));				
			break;

		case 2:	//Spawn right		
			collectable->setCollectableTrans(D3DXVECTOR2(clientBounds.right, collectableTransY));
			collectable->setCollectableVeloc(D3DXVECTOR2(-collectable->getCollectableMaxVeloc().x, collectableVelocY));				
			break;

		case 3:	//Spawn bottom		
			collectable->setCollectableTrans(D3DXVECTOR2(collectableTransX, clientBounds.bottom));
			collectable->setCollectableVeloc(D3DXVECTOR2(collectableVelocX,-collectable->getCollectableMaxVeloc().y));				
			break;

		case 4:	//Spawn left		
			collectable->setCollectableTrans(D3DXVECTOR2(0.0f, collectableTransY));
			collectable->setCollectableVeloc(D3DXVECTOR2(collectable->getCollectableMaxVeloc().x,collectableVelocY));				
			break;			
	}	

	//Boolean to toggle initialisation if reseting, rather than continuing after pause
	bool reset = true;

	//Boolean used to only insert score into scoreboard once
	bool inserted = false;


	ScreenManager screenManager = Menu;	

	// Create the background surface
	aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\stars.jpg");

		

	QueryPerformanceCounter((LARGE_INTEGER*)&previousTime);

	while( msg.message!=WM_QUIT )
	{
		// Check the message queue
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			// Game code goes here

			switch(screenManager)
			{
			case Menu:
				{
					paused = false;
					int const numButtons = 3;
					cButton* buttons[numButtons];
					D3DXVECTOR2 buttonStartPos((clientBounds.right/2) - 45, clientBounds.bottom/3);
					D3DXVECTOR2 buttonOffset(0.0f,100.0f);

					for(int i = 0; i<numButtons; i++)
					{
						buttons[i] = new cButton(d3dMgr, buttonStartPos);
						buttonStartPos += buttonOffset;
					}
					for(int i = 0; i < numButtons; ++i)
					{
						if (buttons[i]->insideRect(buttons[i]->getBoundingRect(),mouseXY))
						{
							buttons[i]->hit();
							mouseXY.x = 0;
							mouseXY.y = 0;
						}
					}

					if (buttons[0]->getHit() == true)
					{
						buttons[0]->hit();
						QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
						timeElapsed = 0.0f;
						previousTime = currentTime;
						reset = true;
						screenManager = Play;
					}
					if (buttons[1]->getHit() == true)
					{
						buttons[1]->hit();
						screenManager = ScoreBoard;
					}
					if (buttons[2]->getHit() == true)
					{
						buttons[1]->hit();
						return 0;
					}					
					

					//Generate UI Text Strings	
					RECT textPos;
					char nameString[50];
					char startString[50];	
					char scoreBoardString[50];	
					char quitString[50];
					sprintf_s( nameString, 50, "Rocket Rescue");
					sprintf_s( startString, 50, "Start");
					sprintf_s( scoreBoardString, 50, "ScoreBoard");
					sprintf_s( quitString, 50, "Quit");

					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(aSurface, theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);
				
					d3dxSRMgr->beginDraw();

					for(int i = 0; i < numButtons; i++)
					{
						d3dxSRMgr->setTheTransform(buttons[i]->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(buttons[i]->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
					}

					d3dxSRMgr->endDraw();

					//Display UI Text
					D3DXVECTOR2 tempPos = buttons[0]->getSpritePos2D();
					
					SetRect(&textPos, tempPos.x - 110 , tempPos.y-70, tempPos.x+200, tempPos.y+60);
					customFont->printText(nameString,textPos);

					SetRect(&textPos, tempPos.x - 65 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(startString,textPos);

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 65 , tempPos.y+5, tempPos.x+210, tempPos.y+60);
					customFont->printText(scoreBoardString,textPos);

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 65 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(quitString,textPos);
					d3dMgr->endRender();										
					
					break;
				}
			case ScoreBoard:
				{
					int const numButtons = 1;
					cButton* buttons[numButtons];
					D3DXVECTOR2 buttonStartPos((clientBounds.right/3) - 45, (clientBounds.bottom/4)*3);
					D3DXVECTOR2 buttonOffset(0.0f,100.0f);
					buttonStartPos +=buttonOffset;

					for(int i = 0; i<numButtons; i++)
					{
						buttons[i] = new cButton(d3dMgr, buttonStartPos);
						buttonStartPos += buttonOffset;
					}
					for(int i = 0; i < numButtons; ++i)
					{
						if (buttons[i]->insideRect(buttons[i]->getBoundingRect(),mouseXY))
						{
							buttons[i]->hit();
							mouseXY.x = 0;
							mouseXY.y = 0;
						}
					}

					
					if (buttons[0]->getHit() == true)
					{
						buttons[0]->hit();
						screenManager = Menu;
					}											
					

					//Generate UI Text Strings	
					RECT textPos;
					char scoreBoardString[50];
					char scoreString1[50];
					char scoreString2[50];
					char scoreString3[50];
					char scoreString4[50];
					char scoreString5[50];
					char retryString[50];
					char menuString[50];					
					char quitString[50];
					sprintf_s( scoreBoardString, 50, "Scoreboard:");

					cScore* tempScore[4];
					int maxScores = 4;
					scoreboard.reset();					
					for(int i = 0; i< maxScores; i++)
					{	
						tempScore[i] = scoreboard.getScores();						
					}

					sprintf_s( scoreString1, 50, "%s: %d", tempScore[0]->getName().c_str(), tempScore[0]->getScore());
					sprintf_s( scoreString2, 50, "%s: %d", tempScore[1]->getName().c_str(), tempScore[1]->getScore());
					sprintf_s( scoreString3, 50, "%s: %d", tempScore[2]->getName().c_str(), tempScore[2]->getScore());
					sprintf_s( scoreString4, 50, "%s: %d", tempScore[3]->getName().c_str(), tempScore[3]->getScore());
					//sprintf_s( scoreString5, 50, "%s: %d", tempScore[4]->getName().c_str(), tempScore[4]->getScore());		
					
					sprintf_s( menuString, 50, "Back");					

					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(aSurface, theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);
				
					d3dxSRMgr->beginDraw();

					for(int i = 0; i < numButtons; i++)
					{
						d3dxSRMgr->setTheTransform(buttons[i]->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(buttons[i]->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
					}

					d3dxSRMgr->endDraw();

					//Display UI Text
					D3DXVECTOR2 tempPos = buttons[0]->getSpritePos2D();
					
					tempPos -= buttonOffset;
					tempPos -= buttonOffset;
					tempPos -= buttonOffset;
					tempPos -= buttonOffset;
					SetRect(&textPos, tempPos.x - 50 , tempPos.y-70, tempPos.x+200, tempPos.y+60);
					customFont->printText(scoreBoardString,textPos);
					
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(scoreString1,textPos);					

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(scoreString2,textPos);

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(scoreString3,textPos);

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(scoreString4,textPos);

					/*tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(scoreString5,textPos);*/					

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(menuString,textPos);
					
					d3dMgr->endRender();
					
					break;
				}
			case Play:
				{
					inserted = false;

					if(paused == true)
					{						
						screenManager = Paused;
					}
					if(reset == true)
					{	
						delete player;
						player = NULL;																
						delete collectable;
						collectable = NULL;

						//Create player object						
						player = new cPlayer(d3dMgr, D3DXVECTOR2(clientBounds.right/2,clientBounds.bottom/2), D3DXVECTOR2(0,0), 0.0f, D3DXVECTOR2 (3.5f,150.0f), D3DXVECTOR2 (0.5f,5.0f), &playerTextures);
	
						//Create asteroid objects
						int const numAsteroids = 5;						
						srand ( (unsigned int)time(NULL) );
						for(int i = 0; i < numAsteroids; i++)
						{
							float asteroidRot = (-1) + static_cast <float>(rand()) / (static_cast <float> (RAND_MAX/ 2));
							asteroidArray[i] = new cAsteroid(d3dMgr, D3DXVECTOR2(-100,-300), D3DXVECTOR2(0.0f,0.0f), asteroidRot, D3DXVECTOR2(100.f,100.f));
							// Generate random variables, and a random spawn position for the asteroid		
							int asteroidCase = rand()%4 + 1;		
							float asteroidVelocX = (-asteroidArray[i]->getAsteroidMaxVeloc().x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(asteroidArray[i]->getAsteroidMaxVeloc().x)*2));
							float asteroidVelocY = (-asteroidArray[i]->getAsteroidMaxVeloc().y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(asteroidArray[i]->getAsteroidMaxVeloc().y)*2));
							float asteroidTransX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.right));
							float asteroidTransY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.bottom));
		  
							switch(asteroidCase)
							{
								case 1:	//Spawn top		
									asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(asteroidTransX, 0.0f));
									asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidVelocX,asteroidArray[i]->getAsteroidMaxVeloc().y));				
									break;

								case 2:	//Spawn right		
									asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(clientBounds.right, asteroidTransY));
									asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(-asteroidArray[i]->getAsteroidMaxVeloc().x, asteroidVelocY));				
									break;

								case 3:	//Spawn bottom		
									asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(asteroidTransX, clientBounds.bottom));
									asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidVelocX,-asteroidArray[i]->getAsteroidMaxVeloc().y));				
									break;

								case 4:	//Spawn left		
									asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(0.0f, asteroidTransY));
									asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidArray[i]->getAsteroidMaxVeloc().x,asteroidVelocY));				
									break;			
							}		
		
						}	

						//Create collectable object
						float collectableRot = (-1) + static_cast <float>(rand()) / (static_cast <float> (RAND_MAX/ 2));
						collectable = new cCollectable(d3dMgr, D3DXVECTOR2(-100,-300), D3DXVECTOR2(0.0f,0.0f), collectableRot, D3DXVECTOR2(50.f,50.f));
						// Generate random variables, and a random spawn position for the collectable		
						int collectableCase = rand()%4 + 1;		
						float collectableVelocX = (-collectable->getCollectableMaxVeloc().x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(collectable->getCollectableMaxVeloc().x)*2));
						float collectableVelocY = (-collectable->getCollectableMaxVeloc().y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(collectable->getCollectableMaxVeloc().y)*2));
						float collectableTransX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.right));
						float collectableTransY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.bottom));
		  
						switch(collectableCase)
						{
							case 1:	//Spawn top		
								collectable->setCollectableTrans(D3DXVECTOR2(collectableTransX, 0.0f));
								collectable->setCollectableVeloc(D3DXVECTOR2(collectableVelocX,collectable->getCollectableMaxVeloc().y));				
								break;

							case 2:	//Spawn right		
								collectable->setCollectableTrans(D3DXVECTOR2(clientBounds.right, collectableTransY));
								collectable->setCollectableVeloc(D3DXVECTOR2(-collectable->getCollectableMaxVeloc().x, collectableVelocY));				
								break;

							case 3:	//Spawn bottom		
								collectable->setCollectableTrans(D3DXVECTOR2(collectableTransX, clientBounds.bottom));
								collectable->setCollectableVeloc(D3DXVECTOR2(collectableVelocX,-collectable->getCollectableMaxVeloc().y));				
								break;

							case 4:	//Spawn left		
								collectable->setCollectableTrans(D3DXVECTOR2(0.0f, collectableTransY));
								collectable->setCollectableVeloc(D3DXVECTOR2(collectable->getCollectableMaxVeloc().x,collectableVelocY));				
								break;			
						}	
						reset = false;
					}					
					
					QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
					float dt = (currentTime - previousTime)*sPC;

					// Accumulate how much time has passed.
					timeElapsed += dt;
				
			
					if(timeElapsed > fpsRate)
					{
						//Update player position
						player->update(spaceButton, upButton, rightButton, leftButton, downButton, timeElapsed);		
						//Update collectable position
						collectable->update(timeElapsed);

						for(int i = 0; i < numAsteroids; i++)
						{	
							//If asteroid is out of bounds, then delete and recreate it.
							D3DXVECTOR2 tempPos = asteroidArray[i]->getSpritePos2D();
							if (tempPos.x>(clientBounds.right + 60) || tempPos.x<(clientBounds.left - 60) ||tempPos.y<(clientBounds.top - 60) ||tempPos.y>(clientBounds.bottom + 60))
							{
								delete asteroidArray[i];
								
								float asteroidRot = (-1) + static_cast <float>(rand()) / (static_cast <float> (RAND_MAX/ 2));
								asteroidArray[i] = new cAsteroid(d3dMgr, D3DXVECTOR2(-100,-300), D3DXVECTOR2(0.0f,0.0f), asteroidRot, D3DXVECTOR2(100.0f,100.0f));
								// Generate random variables, and a random spawn position for the asteroid		
								int asteroidCase = rand()%4 + 1;						
								float asteroidVelocX = (-asteroidArray[i]->getAsteroidMaxVeloc().x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(asteroidArray[i]->getAsteroidMaxVeloc().x)));
								float asteroidVelocY = (-asteroidArray[i]->getAsteroidMaxVeloc().y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(asteroidArray[i]->getAsteroidMaxVeloc().y)));
								float asteroidTransX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.right));
								float asteroidTransY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.bottom));
		  
								switch(asteroidCase)
								{
									case 1:	//Spawn top		
										asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(asteroidTransX, 0.0f));
										asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidVelocX,asteroidArray[i]->getAsteroidMaxVeloc().y));								
										break;

									case 2:	//Spawn right		
										asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(clientBounds.right, asteroidTransY));
										asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(-asteroidArray[i]->getAsteroidMaxVeloc().x, asteroidVelocY));								
										break;

									case 3:	//Spawn bottom		
										asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(asteroidTransX, clientBounds.bottom));
										asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidVelocX,-asteroidArray[i]->getAsteroidMaxVeloc().y));								
										break;

									case 4:	//Spawn left		
										asteroidArray[i]->setAsteroidTrans(D3DXVECTOR2(0.0f, asteroidTransY));
										asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidArray[i]->getAsteroidMaxVeloc().x,asteroidVelocY));								
										break;			
								}						
							}
							//Update asteroid position
							asteroidArray[i]->update(timeElapsed);

							//Check for collisions with asteroids
							for(int i2 = 0; i2 < numAsteroids; ++i2)
							{
								if (player->collidedWith(player->getBoundingRect(),asteroidArray[i2]->getBoundingRect()))
								{
									player->hit();
									if (player->getHealth()<= 0)
									{
   										screenManager = GameOver;
									}
								}
								if (asteroidArray[i]->collidedWith(asteroidArray[i]->getBoundingRect(),asteroidArray[i2]->getBoundingRect()))
								{							
									OutputDebugString("Collision!!");			
									D3DXVECTOR2 tempVector = asteroidArray[i]->getSpritePos2D();
									D3DXVECTOR2 tempVector2 = asteroidArray[i2]->getSpritePos2D();
									D3DXVECTOR2 tempVeloc = asteroidArray[i]->getAsteroidVeloc();
									D3DXVECTOR2 tempVeloc2 = asteroidArray[i2]->getAsteroidVeloc();

									float dx = tempVector2.x - tempVector.x;
									float dy = tempVector2.y - tempVector.y;

									float dx2 = (tempVector2.x + tempVeloc2.x) - (tempVector.x + tempVeloc.x);
									float dy2 = (tempVector2.y + tempVeloc2.y) - (tempVector.y + tempVeloc.y);

									float distance = ((dx*dx) + (dy*dy));
									float distance2 = ((dx2*dx2) + (dy2*dy2));
									if (distance2 < distance)
									{
										if(tempVeloc.x > tempVeloc.y)
										{
											asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2((asteroidArray[i]->getAsteroidVeloc().x*(-1)), asteroidArray[i]->getAsteroidVeloc().y));
										}
										else
										{
											asteroidArray[i]->setAsteroidVeloc(D3DXVECTOR2(asteroidArray[i]->getAsteroidVeloc().x, (asteroidArray[i]->getAsteroidVeloc().y*(-1))));
										}
										if(tempVeloc2.x > tempVeloc2.y)
										{
											asteroidArray[i2]->setAsteroidVeloc(D3DXVECTOR2((asteroidArray[i2]->getAsteroidVeloc().x*(-1)), asteroidArray[i2]->getAsteroidVeloc().y));
										}
										else
										{
											asteroidArray[i2]->setAsteroidVeloc(D3DXVECTOR2(asteroidArray[i2]->getAsteroidVeloc().x, (asteroidArray[i2]->getAsteroidVeloc().y*(-1))));
										}

										asteroidArray[i]->setAsteroidRotSpeed(asteroidArray[i]->getAsteroidRotSpeed()*(-1));
										asteroidArray[i2]->setAsteroidRotSpeed(asteroidArray[i2]->getAsteroidRotSpeed()*(-1));
									}
								}
							}
						}

						//Check for the player colliding with the collectable
						if (player->collidedWith(player->getBoundingRect(),collectable->getBoundingRect()))
						{
							//Increase the player's score
							player->increaseScore();
							//Set collectable out of bounds, to avoid duplicating the delete/recreate code again
							collectable->setSpritePos2D(D3DXVECTOR3(-300,-300, 0));
						}
				
						//If collectable is out of bounds, then delete and recreate it.
						D3DXVECTOR2 tempPosCollectable = collectable->getSpritePos2D();
						if(tempPosCollectable.x>(clientBounds.right + 60) || tempPosCollectable.x<(clientBounds.left - 60) ||tempPosCollectable.y<(clientBounds.top - 60) ||tempPosCollectable.y>(clientBounds.bottom + 60))
						{
							//Delete collectable
							delete collectable;
							//Create collectable object
							float collectableRot = (-1) + static_cast <float>(rand()) / (static_cast <float> (RAND_MAX/ 2));
							collectable = new cCollectable(d3dMgr, D3DXVECTOR2(-100,-300), D3DXVECTOR2(0.0f,0.0f), collectableRot, D3DXVECTOR2(50.f,50.f));
							// Generate random variables, and a random spawn position for the collectable		
							int collectableCase = rand()%4 + 1;		
							float collectableVelocX = (-collectable->getCollectableMaxVeloc().x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(collectable->getCollectableMaxVeloc().x)*2));
							float collectableVelocY = (-collectable->getCollectableMaxVeloc().y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(collectable->getCollectableMaxVeloc().y)*2));
							float collectableTransX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.right));
							float collectableTransY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/clientBounds.bottom));
		  
							switch(collectableCase)
							{
								case 1:	//Spawn top		
									collectable->setCollectableTrans(D3DXVECTOR2(collectableTransX, 0.0f));
									collectable->setCollectableVeloc(D3DXVECTOR2(collectableVelocX,collectable->getCollectableMaxVeloc().y));				
									break;

								case 2:	//Spawn right		
									collectable->setCollectableTrans(D3DXVECTOR2(clientBounds.right, collectableTransY));
									collectable->setCollectableVeloc(D3DXVECTOR2(-collectable->getCollectableMaxVeloc().x, collectableVelocY));				
									break;

								case 3:	//Spawn bottom		
									collectable->setCollectableTrans(D3DXVECTOR2(collectableTransX, clientBounds.bottom));
									collectable->setCollectableVeloc(D3DXVECTOR2(collectableVelocX,-collectable->getCollectableMaxVeloc().y));				
									break;

								case 4:	//Spawn left		
									collectable->setCollectableTrans(D3DXVECTOR2(0.0f, collectableTransY));
									collectable->setCollectableVeloc(D3DXVECTOR2(collectable->getCollectableMaxVeloc().x,collectableVelocY));				
									break;			
							}
						}

						//Restrain player to game window
						D3DXVECTOR3 playerPos = player->getSpritePos2D();				
						if (playerPos.x>(clientBounds.right - 60)) 
						{
							if (playerPos.y<(clientBounds.top))
							{
								player->setShipTrans(D3DXVECTOR2(clientBounds.right - 60, clientBounds.top));
							}
							else
							if (playerPos.y>(clientBounds.bottom - 80))
							{
								player->setShipTrans(D3DXVECTOR2(clientBounds.right - 60, clientBounds.bottom - 80));
							}
							else
							{
								player->setShipTrans(D3DXVECTOR2(clientBounds.right - 60, playerPos.y));
							}
						}
						else
						{
							if (playerPos.x<(clientBounds.left + 40)) 
							{
								if (playerPos.y<(clientBounds.top))
								{
									player->setShipTrans(D3DXVECTOR2(clientBounds.left + 40, clientBounds.top));
								}
								else
								if (playerPos.y>(clientBounds.bottom - 80))
								{
									player->setShipTrans(D3DXVECTOR2(clientBounds.left + 40, clientBounds.bottom - 80));
								}
								else
								{
									player->setShipTrans(D3DXVECTOR2(clientBounds.left + 40, playerPos.y));
								}
							}
							else
							{
								if (playerPos.y<(clientBounds.top))
								{
									player->setShipTrans(D3DXVECTOR2(playerPos.x, clientBounds.top));						
								}
								if (playerPos.y>(clientBounds.bottom - 80))
								{
									player->setShipTrans(D3DXVECTOR2(playerPos.x, clientBounds.bottom - 80));
								}
							}
						}

						//Generate UI Text Strings	
						RECT textPos;	
						char healthString[50];
						char scoreString[50];
						sprintf_s( healthString, 50, "Health : %d", player->getHealth());
						sprintf_s( scoreString, 50, "Score : %d", player->getScore());
				
				

						d3dMgr->beginRender();
						theBackbuffer = d3dMgr->getTheBackBuffer();
						d3dMgr->updateTheSurface(aSurface, theBackbuffer);
						d3dMgr->releaseTheBackbuffer(theBackbuffer);
				
						d3dxSRMgr->beginDraw();

						//Draw player
						d3dxSRMgr->setTheTransform(player->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(player->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);

						//Draw asteroids
						for(int i = 0; i < numAsteroids; i++)
						{
							d3dxSRMgr->setTheTransform(asteroidArray[i]->getSpriteTransformMatrix());  
							d3dxSRMgr->drawSprite(asteroidArray[i]->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
						}

						//Draw collectable
						d3dxSRMgr->setTheTransform(collectable->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(collectable->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);						
				
						d3dxSRMgr->endDraw();

						//Display UI Text
						SetRect(&textPos, 50, 10, 550, 100);
						customFont->printText(healthString,textPos);

						SetRect(&textPos, clientBounds.right - 240, 10, clientBounds.right, 100);
						customFont->printText(scoreString,textPos);

						d3dMgr->endRender();
						timeElapsed = 0.0f;
					}
					previousTime = currentTime;

					break;
				}
			case GameOver:
				{
					paused = false;
					int const numButtons = 3;
					cButton* buttons[numButtons];
					D3DXVECTOR2 buttonStartPos((clientBounds.right/2) - 45, clientBounds.bottom/3);
					D3DXVECTOR2 buttonOffset(0.0f,100.0f);
					buttonStartPos +=buttonOffset;

					for(int i = 0; i<numButtons; i++)
					{
						buttons[i] = new cButton(d3dMgr, buttonStartPos);
						buttonStartPos += buttonOffset;
					}
					for(int i = 0; i < numButtons; ++i)
					{
						if (buttons[i]->insideRect(buttons[i]->getBoundingRect(),mouseXY))
						{
							buttons[i]->hit();
							mouseXY.x = 0;
							mouseXY.y = 0;
						}
					}

					
					if (buttons[0]->getHit() == true)
					{
						buttons[0]->hit();
						QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
						timeElapsed = 0.0f;
						previousTime = currentTime;
						reset = true;
						screenManager = Play;
					}

					if (buttons[1]->getHit() == true)
					{
						buttons[1]->hit();
						screenManager = Menu;
					}
					if (buttons[2]->getHit() == true)
					{
						buttons[2]->hit();							
						return 0;
					}

					if (inserted == false)
					{
						cScore* score = new cScore(player->getScore(),"Player");
						scoreboard.insertScore(score);	
						inserted = true;
					}
					

					//Generate UI Text Strings	
					RECT textPos;
					char gameOverString[50];
					char scoreString[50];
					char retryString[50];
					char menuString[50];					
					char quitString[50];
					sprintf_s( gameOverString, 50, "Game Over!");
					sprintf_s( scoreString, 50, "Score : %d", player->getScore());
					sprintf_s( retryString, 50, "Retry");
					sprintf_s( menuString, 50, "Menu");
					sprintf_s( quitString, 50, "Quit");

					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(aSurface, theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);
				
					d3dxSRMgr->beginDraw();

					for(int i = 0; i < numButtons; i++)
					{
						d3dxSRMgr->setTheTransform(buttons[i]->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(buttons[i]->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
					}

					d3dxSRMgr->endDraw();

					//Display UI Text
					D3DXVECTOR2 tempPos = buttons[0]->getSpritePos2D();
					
					tempPos -= buttonOffset;
					SetRect(&textPos, tempPos.x - 50 , tempPos.y-70, tempPos.x+200, tempPos.y+60);
					customFont->printText(gameOverString,textPos);
					
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(scoreString,textPos);

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(retryString,textPos);

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(menuString,textPos);

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(quitString,textPos);
					d3dMgr->endRender();

					break;
				}
			case Paused:
				{
					int const numButtons = 2;
					cButton* buttons[numButtons];
					D3DXVECTOR2 buttonStartPos((clientBounds.right/2) - 45, clientBounds.bottom/3);
					D3DXVECTOR2 buttonOffset(0.0f,100.0f);

					for(int i = 0; i<numButtons; i++)
					{
						buttons[i] = new cButton(d3dMgr, buttonStartPos);
						buttonStartPos += buttonOffset;
					}
					for(int i = 0; i < numButtons; ++i)
					{
						if (buttons[i]->insideRect(buttons[i]->getBoundingRect(),mouseXY))
						{
							buttons[i]->hit();
							mouseXY.x = 0;
							mouseXY.y = 0;
						}
					}

					if (buttons[0]->getHit() == true)
					{
						buttons[0]->hit();
						paused = false;
					}
					if (buttons[1]->getHit() == true)
					{
						buttons[1]->hit();							
						screenManager = Menu;
					}
					
					if(paused == false)
					{
						QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
						timeElapsed = 0.0f;
						previousTime = currentTime;
						screenManager = Play;
						reset = false;
					}

					//Generate UI Text Strings	
					RECT textPos;
					char pausedString[50];
					char continueString[50];					
					char quitString[50];
					sprintf_s( pausedString, 50, "Paused");
					sprintf_s( continueString, 50, "Continue");
					sprintf_s( quitString, 50, "Menu");

					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(aSurface, theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);
				
					d3dxSRMgr->beginDraw();

					for(int i = 0; i < numButtons; i++)
					{
						d3dxSRMgr->setTheTransform(buttons[i]->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite(buttons[i]->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
					}

					d3dxSRMgr->endDraw();

					//Display UI Text
					D3DXVECTOR2 tempPos = buttons[0]->getSpritePos2D();
					
					SetRect(&textPos, tempPos.x - 45 , tempPos.y-70, tempPos.x+200, tempPos.y+60);
					customFont->printText(pausedString,textPos);

					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(continueString,textPos);

					tempPos += buttonOffset;
					SetRect(&textPos, tempPos.x - 45 , tempPos.y+5, tempPos.x+200, tempPos.y+60);
					customFont->printText(quitString,textPos);
					d3dMgr->endRender();

					break;
				}
			}			
		}
	}
	
	d3dxSRMgr->cleanUp();
	d3dMgr->clean();
	return (int) msg.wParam;
}

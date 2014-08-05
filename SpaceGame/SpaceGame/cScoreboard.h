#ifndef _CSCOREBOARD_H
#define _CSCOREBOARD_H
#include "GameConstants.h"
#include "cScore.h"

class cScoreboard
{
private:
	cScore* pTopScore;
	cScore* pTempScore;
	
public:
	cScoreboard(cScore* pFirstScore);//Constructor	
	~cScoreboard();//Destructor	
	void insertScore(cScore* pNewScore);
	cScore* getTempScore();
	cScore* getScores();
	void reset();
};
#endif
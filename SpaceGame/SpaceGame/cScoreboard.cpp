#include "cScoreboard.h"

cScoreboard::cScoreboard(cScore* pFirstScore)
{
	pTopScore = pFirstScore;
	pTempScore = pTopScore;
}

cScoreboard::~cScoreboard()
{
}

void cScoreboard::insertScore(cScore* pNewScore)
{
	pTempScore = pTopScore;
	if((pTopScore->getScore()) < (pNewScore->getScore()))
	{
		pNewScore->setNextScore(pTopScore);
		pTopScore = pNewScore;
	}
	else
	{
		bool scoreInserted = false;
		while (scoreInserted == false)
		{
			if(pTempScore->getNextScore() != NULL)
			{			
				if((pTempScore->getNextScore()->getScore()) < (pNewScore->getScore()))
				{
					pNewScore->setNextScore(pTempScore->getNextScore());
					pTempScore->setNextScore(pNewScore);
					scoreInserted = true;
				}
				else
				{
					pTempScore = pTempScore->getNextScore();
				}
			}		
			else
			{
				pTempScore->setNextScore(pNewScore);
				scoreInserted = true;
			}
		}
	}
}
cScore* cScoreboard:: getTempScore()
{
	return pTempScore;
}

cScore* cScoreboard::getScores()
{
	if(pTempScore == pTopScore)
	{
		pTempScore = pTempScore->getNextScore();
		return pTopScore;
	}
	else
	{	
		cScore* temp = pTempScore;
		pTempScore = pTempScore->getNextScore();
		return temp;				
	}
	
}

void cScoreboard::reset()
{
	pTempScore = pTopScore;
}
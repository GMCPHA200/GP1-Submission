#ifndef _CSCORE_H
#define _CSCORE_H
#include "GameConstants.h"

class cScore
{
private:
	int score;
	std::string name;
	cScore* nextScore;
public:
	cScore(int score, std::string name);//Constructor	
	~cScore();//Destructor
	int getScore();
	std::string getName();
	cScore* getNextScore();
	void setNextScore(cScore* newNextScore);
};
#endif
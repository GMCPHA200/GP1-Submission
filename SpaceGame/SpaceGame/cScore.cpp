#include "cScore.h"

cScore::cScore(int newScore, std::string newName)
{
	score = newScore;
	name = newName;
	nextScore = NULL;
}

cScore::~cScore()
{
}

int cScore::getScore()
{
	return score;
}
std::string cScore::getName()
{
	return name;
}
cScore* cScore::getNextScore()
{
	return nextScore;
}
void cScore::setNextScore(cScore* newNextScore)
{
	nextScore = newNextScore;
}

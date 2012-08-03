#include "Game.h"

class GameFruit : public Game
{
	public:
	GameFruit();
	virtual int getSizeToAdd();
	virtual void createFood();
	virtual int getMinFood();
};


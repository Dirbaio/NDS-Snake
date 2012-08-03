#include "Game.h"

class GameEscape : public Game
{
	public:
	point foodPos;
	
	GameEscape();
	virtual void createFood();
};


#ifndef _SNAKE_H
#define _SNAKE_H
#include "util.h"

#define SNAKE_HUMAN 0
#define SNAKE_INV 1
#define SNAKE_AI 2

class Game;

class Snake
{
	public: 
	
	Game* g;
	
	point snake[MAXLEN];
	int snakeStart, snakeEnd, snakeDir;
	
	point pos;
	int dir;
	int sizeToAdd;
	int controlMode;

	bool dead;
	int num;
	
	Snake(Game* g, int controlMode, point pos, int dir, int num);
	
	void start();
	void tick();
	
	int getDirKeys();

	int getSpaceAroundPos(point p);
	int getDirAI();
};

#endif


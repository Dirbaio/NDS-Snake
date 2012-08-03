#ifndef _FOOD_H
#define _FOOD_H

#include "util.h"

#define FOOD_NORMAL 0
#define FOOD_RANDMOVE 1
#define FOOD_LINEMOVE 2
#define FOOD_LINEMOVE2 3
#define FOOD_LINEMOVE3 4
#define FOOD_LINEMOVE4 5

class Game;

class Food
{
	public:
	
	point p;
	int size, type;
	bool eaten;

	Game* g;
	
	Food(Game* g, point p);
	Food(Game* g, point p, int size, int type);
	
	virtual void tick();
	virtual void render();
	
	bool contains(point b);
};

#endif

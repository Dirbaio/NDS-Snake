#ifndef _GAME_H
#define _GAME_H

#include<nds.h>
#include "Snake.h"
#include "Food.h"
#include<list>
#include "Scene.h"

using namespace std;

#define SNAKE_COUNT 2

class Game : public Scene
{
	public:
	
	u8 tab[WIDTH][HEIGHT];

	int score;
	
	Snake* snakes[SNAKE_COUNT];
	list<Food> foods;
	
	//==========
	
	Game();
	virtual ~Game();
	
	virtual void start();
	virtual void tick();
	virtual int getSpeed();

	virtual void onStart();
	virtual void onTick();
	virtual int getSizeToAdd();
	virtual void createFood();
	virtual int getMinFood();
	virtual void onEatFood();

	bool isDead();
	void set(point p, int t);
	void setWall(point p, bool b);
	int get(point p);
	bool getPassable(point p);
	
	void addFood(point p, int s);
	point getFreeSpace(int s);
	point randomPosWith(int t);
	int countPointsWith(int t);

};

inline void Game::set(point p, int t)
{
	tab[p.x][p.y] = t;
}

inline int Game::get(point p)
{
	return tab[p.x][p.y];
}

inline bool Game::getPassable(point p)
{
	int what = tab[p.x][p.y];
	return what != WALL && what != BODY && what != HEAD && what != BODY2 && what != HEAD2;
}

#endif

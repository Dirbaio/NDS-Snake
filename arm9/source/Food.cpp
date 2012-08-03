#include "Food.h"
#include "Game.h"
#include<stdio.h>
Food::Food(Game* g, point p)
{
	this->g = g;
	this->p = p;
	this->size = 1;
	this->type = 0;
	eaten = false;
}

Food::Food(Game* g, point p, int size, int type)
{
	this->g = g;
	this->p = p;
	this->size = size;
	this->type = type;
	eaten = false;
}

void Food::tick()
{
	if(type == FOOD_RANDMOVE)
	{
		int dir = rand()%4;
	
		point newPos = p;
		if(dir == 0) newPos.x++;
		if(dir == 1) newPos.y++;
		if(dir == 2) newPos.y--;
		if(dir == 3) newPos.x--;
	
		wrap(newPos);
	
		if(g->get(newPos) == BG)
			p = newPos;
	}
}

void Food::render()
{
	for(int x = 0; x < size; x++)
		for(int y = 0; y < size; y++)
			setTile(x+p.x, y+p.y, FOOD);
}

bool Food::contains(point b)
{
	if(b.x < p.x) return false;
	if(b.y < p.y) return false;
	if(b.x >= p.x+size) return false;
	if(b.y >= p.y+size) return false;
	return true;
}


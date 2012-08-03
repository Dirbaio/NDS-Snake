#include "Snake.h"
#include "Game.h"
#include<queue>
#include<stdio.h>

Snake::Snake(Game* g, int controlMode, point pos, int dir, int num)
{
	this->g = g;
	this->pos = pos;
	this->dir = dir;
	this->num = num;
	
	sizeToAdd = 40;
	
	snakeStart = 0;
	snakeEnd = 0;
	snakeDir = 1;

	snake[snakeStart++] = pos;
	this->controlMode = controlMode;
	
	dead = false;
}

int Snake::getDirKeys()
{
	int ndir = dir;
	if(((keys & KEY_X) || (keys & KEY_UP)) && dir != 2) ndir = 0;
	if(((keys & KEY_B) || (keys & KEY_DOWN)) && dir != 0) ndir = 2;
	if(((keys & KEY_Y) || (keys & KEY_LEFT)) && dir != 1) ndir = 3;
	if(((keys & KEY_A) || (keys & KEY_RIGHT)) && dir != 3) ndir = 1;
	return ndir;
}

point aiQueue[MAXLEN*2];
const int lostDirs[4] = {2, 1, 3, 0};
u8 dist[WIDTH][HEIGHT];

int Snake::getSpaceAroundPos(point p)
{
	memset(dist, 0, sizeof(dist));

	int res = 0;
	
	int qFront = 0;
	int qBack = 0;

	if(g->getPassable(p))
	{
		dist[p.x][p.y] = 1;
		aiQueue[qBack++] = p;
	}
	
	while(qFront < qBack)
	{
		res++;
		p = aiQueue[qFront++];
		for(int i = 0; i < 4; i++)
		{
			point p2 = p;
			p2.x += dxs[i];
			p2.y += dys[i];
			wrap(p2);

			if(g->getPassable(p2) && dist[p2.x][p2.y] == 0)
			{
				dist[p2.x][p2.y] = 1;
				aiQueue[qBack++] = p2;
			}
		}
	}
	
	return res;
}

int Snake::getDirAI()
{
	//Detect "dead ends" and don't go into them.	
	bool canGo[4];
	int spaces[4];
	for(int i = 0; i < 4; i++)
	{
		point p2 = pos;
		p2.x += dxs[i];
		p2.y += dys[i];
		wrap(p2);
		
		int len = (snakeStart - snakeEnd + MAXLEN)%MAXLEN;
		int space = getSpaceAroundPos(p2);
		spaces[i] = space;
//		printf("%d %d\n", space, len);
		canGo[i] = space > len && space > 10;
	}
	
	//Yay, BFS AI FTW.

	int qFront = 0;
	int qBack = 0;
	
	memset(dist, 255, sizeof(dist));
	
	for(list<Food>::iterator it = g->foods.begin(); it != g->foods.end(); it++)
	{
		int x = it->p.x;
		int y = it->p.y;
		int s = it->size;
		for(int xx = 0; xx < s; xx++)
			for(int yy = 0; yy < s; yy++)
			{
				dist[x+xx][y+yy] = 0;
				aiQueue[qBack++] = {x+xx, y+yy};
			}
	}
	
	while(qFront < qBack)
	{
		point p = aiQueue[qFront++];
		int d = dist[p.x][p.y];
		for(int i = 0; i < 4; i++)
		{
			point p2 = p;
			p2.x += dxs[i];
			p2.y += dys[i];
			wrap(p2);
			
			if(g->getPassable(p2))
			{
				if(dist[p2.x][p2.y] == -1 || dist[p2.x][p2.y] > d+1)
				{
					dist[p2.x][p2.y] = d+1;
					aiQueue[qBack++] = p2;
				}
			}
		}
	}
	
	int best = 255;
	int bestd = -1;
	
	int dd;
	
	dd = dist[pos.x][(pos.y-1+HEIGHT)%HEIGHT]*2; if(dir == 0) dd--; if(canGo[0] && dd < best && dir != 2) {best = dd; bestd = 0; }
	dd = dist[pos.x][(pos.y+1+HEIGHT)%HEIGHT]*2; if(dir == 2) dd--; if(canGo[2] && dd < best && dir != 0) {best = dd; bestd = 2; }
	dd = dist[(pos.x-1+WIDTH)%WIDTH]  [pos.y]*2; if(dir == 3) dd--; if(canGo[3] && dd < best && dir != 1) {best = dd; bestd = 3; }
	dd = dist[(pos.x+1+WIDTH)%WIDTH]  [pos.y]*2; if(dir == 1) dd--; if(canGo[1] && dd < best && dir != 3) {best = dd; bestd = 1; }
	
	if(bestd != -1)
		return bestd;
	
	// Else, the food is blocked off. Then just find a direction that 
	// isn't blocked because the dist table is going to do no shit for us.
	/*
	for(int i = 0; i < 4; i++)
	{
		int d = lostDirs[i];
		
		if(!canGo[d])
			continue;
			
		point p2 = pos;
		p2.x += dxs[d];
		p2.y += dys[d];
	
		wrap(p2);
	
		if(g->getPassable(p2))
			return d;
	}*/

	//Second try. Find the direction that is the least blocked.
		
	best = -1; 
	bestd = -1;

	for(int i = 0; i < 4; i++)
	{
		int d = lostDirs[i];
		
		point p2 = pos;
		p2.x += dxs[d];
		p2.y += dys[d];
	
		wrap(p2);
	
		if(g->getPassable(p2) && spaces[d] > best)
		{
			best = spaces[d];
			bestd = d;
		}
	}

	if(bestd != -1)
		return bestd;

	//If we get here we're blocked in all directions.
	//So we're screwed anyway.
	return dir;
}

void Snake::tick()
{
	if(dead) return;
	
	//Read Keys.
	if(controlMode == SNAKE_AI)
		dir = getDirAI();
	else
		dir = getDirKeys();
		
	//Grow snake head.
	point oldpos = pos;
	
	pos.x = (pos.x + dxs[dir] + WIDTH) % WIDTH;
	pos.y = (pos.y + dys[dir] + HEIGHT) % HEIGHT;

	snake[snakeStart] = pos;
	snakeStart = (snakeStart + snakeDir + MAXLEN) % MAXLEN;
	
	if(!g->getPassable(pos))
	{
		dead = true;
		return;
	}

	g->set(oldpos, BODY+num*2);
	g->set(pos, HEAD+num*2);
	
	for(list<Food>::iterator it = g->foods.begin(); it != g->foods.end(); it++)
	{
		if(it->contains(pos))
		{
			sizeToAdd += g->getSizeToAdd();
			g->score += g->getSizeToAdd();
			g->onEatFood();
			it->eaten = true;
		}
	}
	
//	sizeToAdd+=10;
	
	int sizeToAdd2 = sizeToAdd/10;
	
	if(sizeToAdd2 <= 0)
	{
		int toShrink = 1;
		if(sizeToAdd2 < 0)
		{
			toShrink++;
			sizeToAdd+=10;
		}
		
		for(int i = 0; i < toShrink; i++)
		{
			g->set(snake[snakeEnd], BG);
			snakeEnd = (snakeEnd + snakeDir + MAXLEN) % MAXLEN;
		}
	}
	else
		sizeToAdd-=10;
}


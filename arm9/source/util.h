#ifndef _UTIL_H
#define _UTIL_H

#include<nds.h>
#include<stdio.h>


#define WIDTH 32
#define HEIGHT 24
#define MAXLEN (WIDTH*HEIGHT+1)

#define BG 0 
#define FOOD 1
#define WALL 2
#define TEXT 3
#define HEAD 4
#define BODY 5
#define HEAD2 6
#define BODY2 7

#define PAL_LEN 8

const int dxs[] = {0, 1, 0, -1};
const int dys[] = {-1, 0, 1, 0};

struct point
{
	int x, y;
};

inline void setTile(int x, int y, int t)
{
	u16* map = (u16*) 0x06010000;
	map[x+y*32] = t;
}
inline void setTile(point p, int t)
{
	setTile(p.x, p.y, t);
}


inline void fillTile(int t, int col)
{
	u16* gfx = (u16*) 0x06000000;

	for(int i = 0; i < 32; i++)
		gfx[t*32+i] = (u16) (col | (col<<8));
}


inline void wrap(point& p)
{
	p.x = (p.x + WIDTH) % WIDTH;
	p.y = (p.y + HEIGHT) % HEIGHT;	
}

//Some global variables
//Derp.

extern int keys;
extern int currGamemode;

class Scene;
Scene* createGame(int gamemode);

#endif

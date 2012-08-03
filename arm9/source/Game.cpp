#include "Game.h"
#include "util.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>


Game::Game()
{
	palette[BG] = RGB15(31, 25, 0);
	palette[HEAD] = RGB15(31, 8, 0);
	palette[BODY] = RGB15(31, 29, 20);
	palette[FOOD] = RGB15(25, 8, 0);
	palette[WALL] = RGB15(19, 16, 12);

	needsPalChange = true;
	
	score = 0;
	
	for(int i = 0; i < SNAKE_COUNT; i++)
		snakes[i] = NULL;
		
	snakes[0] = new Snake(this, SNAKE_HUMAN, {10, 10}, 1, 0);
	
	for(int y = 0; y < HEIGHT; y++)
		for(int x = 0; x < WIDTH; x++)
			set({x, y}, BG);
}

Game::~Game()
{
	for(int i = 0; i < SNAKE_COUNT; i++)
		if(snakes[i])
			delete snakes[i];
}

void Game::start()
{
	onStart();
	onEatFood();
}

void Game::tick()
{
	onTick();
	
	for(int i = 0; i < SNAKE_COUNT; i++)
		if(snakes[i])
			snakes[i]->tick();
	
	for(list<Food>::iterator it = foods.begin(); it != foods.end();)
	{
		if(it->eaten)
		{
			list<Food>::iterator it2 = it;
			it2++;
			foods.erase(it);
			it = it2;
		}
		else
		{
			it->tick();
			it++;
		}
	}
	
	while(foods.size() < getMinFood())
		createFood();

	//Render everything
	for(int y = 0; y < HEIGHT; y++)
		for(int x = 0; x < WIDTH; x++)
			setTile(x, y, tab[x][y]);
	
	for(list<Food>::iterator it = foods.begin(); it != foods.end(); it++)
		it->render();

	if(snakes[0]->dead)
		doSceneChange(new Game());
}

//==================

void Game::onStart() {}
void Game::onTick() {}

int Game::getSizeToAdd()
{
	return 30;
}

int Game::getMinFood()
{
	return 1;
}

void Game::onEatFood() {}

void Game::createFood()
{
	foods.push_back(Food(this, getFreeSpace(1), 1, FOOD_NORMAL));
}

int Game::getSpeed()
{
	if(score < 100) return 15;
	if(score < 400) return 12;
	if(score < 1000) return 9;
	if(score < 2000) return 6;
	if(score < 3500) return 5;
	if(score < 4500) return 3;
	return 0;
}

//==================

int Game::countPointsWith(int t)
{
	int r = 0;

	for(int y = 0; y < HEIGHT; y++)
		for(int x = 0; x < WIDTH; x++)
			if(tab[x][y] == t)
				r++;
	return r;
}

point Game::randomPosWith(int t)
{
	int ct = countPointsWith(t);
	
	if(ct == 0)
	{
		point res;
		res.x = rand() % WIDTH;
		res.y = rand() % HEIGHT;
		return res;
	}

	if(ct < 40)
	{
		int r = rand() % (WIDTH*HEIGHT);
		while(tab[r%WIDTH][r/WIDTH] != t)
			r++;
		
		return {r%WIDTH, r/WIDTH};
	}
	
	point res;
	res.x = rand() % WIDTH;
	res.y = rand() % HEIGHT;
	
	while(get(res) != t)
	{
		res.x = rand() % WIDTH;
		res.y = rand() % HEIGHT;
	}
	return res;
}

point Game::getFreeSpace(int s)
{
	point res;
	res.x = rand() % (WIDTH-s);
	res.y = rand() % (HEIGHT-s);
	
	int tries = 0;
	while(tries < 50)
	{
		tries++;
		
		bool ok = true;
		
		for(int x = 0; x < s; x++)
			for(int y = 0; y < s; y++)
				if(get({res.x+x, res.y+y}) != BG)
					ok = false;

		if(ok) break;
			
		res.x = rand() % (WIDTH-s);
		res.y = rand() % (HEIGHT-s);
	}
	
	return res;
}


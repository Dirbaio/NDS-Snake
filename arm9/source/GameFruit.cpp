#include "GameFruit.h"

GameFruit::GameFruit()
{
	palette[0] = RGB15(0, 17, 4);
	palette[1] = RGB15(31, 17, 0);
	palette[2] = RGB15(16, 31, 20);
}

int GameFruit::getSizeToAdd()
{
	return 4;
}

void GameFruit::createFood()
{
	point res = getFreeSpace(3);
	
	for(int x = 0; x < 3; x++)
		for(int y = 0; y < 3; y++)
			if(x == 1 && y == 1)
				set({res.x+x, res.y+y}, WALL);
			else
				foods.push_back(Food(this, {res.x+x, res.y+y}, 1, FOOD_NORMAL));
}

int GameFruit::getMinFood()
{
	return 3;
}


#include "GameEscape.h"

GameEscape::GameEscape()
{
	palette[0] = RGB15(5, 0, 12);
	palette[1] = RGB15(4, 24, 2);
	palette[2] = RGB15(8, 12, 23);
}

void GameEscape::createFood()
{
	foods.push_back(Food(this, getFreeSpace(1), 1, FOOD_RANDMOVE));
}


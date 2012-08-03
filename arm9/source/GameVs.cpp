#include "GameVs.h"

GameVs::GameVs()
{
	palette[BG] = RGB15(0, 0, 0);
	palette[HEAD2] = RGB15(31, 8, 0);
	palette[BODY2] = RGB15(25, 4, 0);
	palette[HEAD] = RGB15(0, 8, 31);
	palette[BODY] = RGB15(0, 4, 25);
	palette[FOOD] = RGB15(16, 31, 0);
	palette[WALL] = RGB15(19, 16, 12);
	palette[TEXT] = RGB15(26, 27, 31);

	snakes[1] = new Snake(this, SNAKE_AI, {20, 10}, 3, 1);	
}

int GameVs::getMinFood()
{
	return 2;
}

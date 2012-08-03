#include "Menu.h"

Menu::Menu()
{
	topGame = new Game();
	palette = topGame->palette;
	needsPalChange = true;
}

Menu::~Menu()
{
	
}

void Menu::start()
{
	topGame->start();
}

void Menu::tick()
{
	topGame->tick();
}

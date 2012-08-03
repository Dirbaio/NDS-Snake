#include<nds.h>
#include "util.h"
#include "Scene.h"
#include "Game.h"

#ifndef _MENU_H
#define _MENU_H

class Menu : public Scene
{
	public:

	Game* topGame;
	
	Menu();
	virtual ~Menu();
	
	virtual void start();
	virtual void tick();
};

#endif

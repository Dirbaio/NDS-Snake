#include <nds.h>
#include <cstdio>
#include <cstdlib>
#include "util.h"
#include "Scene.h"
#include "Menu.h"
#include "Game.h"
#include "GameFruit.h"
#include "GameEscape.h"
#include "GameVs.h"


int rold[PAL_LEN], rnew[PAL_LEN];
int gold[PAL_LEN], gnew[PAL_LEN];
int bold[PAL_LEN], bnew[PAL_LEN];
int palTransitionTime;

#define TRANSLEN 60

int keys;


void tickTransition()
{
	if(palTransitionTime > 0)
	{
		palTransitionTime--;

		for(int i = 0; i < PAL_LEN; i++)
		{
			int r = (rnew[i] * (TRANSLEN - palTransitionTime) + rold[i] * palTransitionTime) / TRANSLEN;
			int g = (gnew[i] * (TRANSLEN - palTransitionTime) + gold[i] * palTransitionTime) / TRANSLEN;
			int b = (bnew[i] * (TRANSLEN - palTransitionTime) + bold[i] * palTransitionTime) / TRANSLEN;
			
			BG_PALETTE[i] = BG_PALETTE_SUB[i] = (r& 0x1F) <<10 | (g & 0x1F) << 5 | (b & 0x1F);
		}
	}
	BG_PALETTE_SUB[0xFF] = BG_PALETTE_SUB[TEXT];
}

void doTransition(Scene* s)
{
	if(!s->needsPalChange)
		return;
	
	for(int i = 0; i < PAL_LEN; i++)
	{
		rold[i] = (BG_PALETTE[i] >> 10) & 0x1F;
		gold[i] = (BG_PALETTE[i] >> 5) & 0x1F;
		bold[i] = (BG_PALETTE[i] >> 0) & 0x1F;

		rnew[i] = (s->palette[i] >> 10) & 0x1F;
		gnew[i] = (s->palette[i] >> 5) & 0x1F;
		bnew[i] = (s->palette[i] >> 0) & 0x1F;
	}
	
	palTransitionTime = TRANSLEN;
}

int currGamemode = -1;

Scene* createGame(int gamemode)
{
	currGamemode = gamemode;
	switch(gamemode)
	{
		case 0: return new Game();
		case 1: return new GameVs();
		case 2: return new GameFruit();
		case 3: return new GameEscape();
		default:
			printf("Invalid gamemode %d\n", gamemode);
			return NULL;
	}
}

int main(void)
{
	consoleDemoInit();
	vramSetBankA(VRAM_A_MAIN_BG);
	printf("Hello World\nLOLSnake by Dirbaio v0.0\n");
	
	for(int i = 0; i < PAL_LEN; i++)
		BG_PALETTE[i] = 0;
	
	REG_DISPCNT = MODE_5_2D | DISPLAY_SCREEN_BASE(1);
	bgInit(2, BgType_ExRotation, BgSize_ER_256x256, 0, 0);
	bgShow(2);
	
	for(int i = 0; i < PAL_LEN; i++)
		fillTile(i, i);

	srand(122);

	Scene* s = new Menu();
	s->start();
	doTransition(s);
	
	while(1)
	{
		keys = 0;

		int ticks = s->getSpeed();;
		if(ticks < 1) ticks = 1;

		for(int i = 0; i < ticks; i++)
		{
			scanKeys();
			keys |= keysHeld();
			tickTransition();
			swiWaitForVBlank();
		}

		s->tick();
		
		if(s->nextScene)
		{
			Scene* old = s;
			s = old->nextScene;
			s->start();
			doTransition(s);
			delete old;
		}
	}
}

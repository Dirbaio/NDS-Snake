#include<nds.h>
#include "util.h"

#ifndef _SCENE_H
#define _SCENE_H

class Scene
{
	public:

	u16 palette[PAL_LEN];
	bool needsPalChange;
	Scene* nextScene;
	
	Scene();
	virtual ~Scene();
	
	virtual void start();
	virtual void tick();
	virtual int getSpeed();

	void doSceneChange(Scene* s);
	
};

#endif

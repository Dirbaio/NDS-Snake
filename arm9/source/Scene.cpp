#include "Scene.h"

Scene::Scene()
{
	needsPalChange = false;
	nextScene = NULL;
}

Scene::~Scene() {}
void Scene::start() {}
void Scene::tick() {}
int  Scene::getSpeed() {return 1;}

void Scene::doSceneChange(Scene* s)
{
	nextScene = s;
}

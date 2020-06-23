#include "Scene.h"

Scene::Scene() {}

GameState Scene::getState() { return stateScene; } //retorna l'estat de la escena

Scene::~Scene(){ delete mixer; }

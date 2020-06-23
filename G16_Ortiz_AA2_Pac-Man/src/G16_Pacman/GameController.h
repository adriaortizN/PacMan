#pragma once
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <exception>
#include <iostream>
#include <time.h>

#include "constants.h"
#include "Inputs.h"
#include "Scene.h"
#include "Menu.h"
#include "Ranking.h"
#include "SplashScreen.h"
#include "Game.h"

class GameController
{
	Inputs * inputs; //inputs
	Scene * currentScene; //escena actual
	GameState gameState; //estat del joc
	std::pair<std::string, int> winner;

public:
	GameController();
	void GameLoop(); //loop
	void Update(); //update
	void Draw(); //draw
	~GameController();
};


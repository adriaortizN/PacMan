#pragma once
#include "Constants.h"
#include <iostream>
#include <SDL_image.h>
#include "Renderer.h"
#include "Inputs.h"
#include "Mixer.h"
#include "TextTexture.h"

class Scene
{
protected:

	std::string backgroundTexture; //fons de la escena
	Mixer * mixer;

	GameState stateScene; //estat de la escena

public:
	Scene();
	GameState getState(); //retorna l'estat
	virtual void Update(InputStruct) = 0; //virtual pura de update
	virtual void Draw() = 0; //virtual pura de draw
	~Scene();
};


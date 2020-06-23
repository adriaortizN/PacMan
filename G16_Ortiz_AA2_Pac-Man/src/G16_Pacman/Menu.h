#pragma once
#include "Scene.h"
#include "Button.h"
class Menu : public Scene
{
private:
	TextTexture * title; //titol del joc
	Button *soundButton, *rankingButton, *playButton, *exitButton; //botons del menu
	SDL_Color * background;
public:
	Menu();
	void Update(InputStruct); //update
	void Draw(); //draw
	~Menu();
};


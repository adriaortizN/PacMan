#pragma once
#include "Scene.h"
#include "Button.h"
#include <fstream>

class Ranking : public Scene
{
private:
	TextTexture * title; //titol de la escena
	Button *exitButton; //botons del ranking
	SDL_Color * background;	//color de fons del ranking

	std::vector<std::pair<std::string, int>> rank;
	std::vector<TextTexture*> scores;

public:
	Ranking(std::pair<std::string, int>);
	void Update(InputStruct);				//update
	void Draw();							//draw
	void CreateRankingFile();				//crea el ranking si no hi ha cap
	void OrderRanking();
	void SaveRanking();

	~Ranking();
};


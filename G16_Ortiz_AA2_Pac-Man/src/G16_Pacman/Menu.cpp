#include "Menu.h"


//Creación del tamaño de los botones 
Menu::Menu() : playButton(new Button{ "Play", Vector2{ 390, 200 } }),
soundButton(new Button("Sound on", Vector2{ 350, 300 })),
rankingButton(new Button("Ranking", Vector2{ 350, 400 })),
exitButton(new Button("Exit", Vector2{ 390, 500 })),
title(new TextTexture{ {"Title Text", "Pacman 123", COLOR_GREEN }, {SCREEN_WIDTH / 2, 50}, TITLE_FONT, ALIGN::CENTER })	//Titulo del juego 
{
	stateScene = GameState::MENU;
	mixer = new Mixer{ AUDIO_PATH, AUDIO_FREQUENCY };
	Renderer::Instance()->LoadTexture("BG_GENERAL", BACKGROUND_PATH); //posa la textura del joc en memoria
}


void Menu::Update(InputStruct inputs) 
{
	playButton->Update(inputs); //actualitza el boto i si es pressiona passa a l'estat GAME
	if (playButton->IsPressed()) stateScene = GameState::GAME;
	soundButton->Update(inputs);
	if (soundButton->IsPressed()) {
		bool isOff = mixer->ToggleAudio();
		isOff ? soundButton->UpdateText("Sound Off") : soundButton->UpdateText("Sound On");
	}

	rankingButton->Update(inputs);
	if (rankingButton->IsPressed()) stateScene = GameState::RANKING;	//Ranking no funcional, per tant, no porta a cap lloc 

	exitButton->Update(inputs);
	if (exitButton->IsPressed()) stateScene = GameState::EXIT; //actualitza el boto i si es pressiona passa a l'estat EXIT
}

void Menu::Draw() { //draw

	Renderer* rend = Renderer::Instance();
	rend->PushImage("BG_GENERAL", Rect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT });
	title->Draw();
	playButton->Draw();
	soundButton->Draw();
	rankingButton->Draw();
	exitButton->Draw();
}

Menu::~Menu()
{
}

#include "SplashScreen.h"

SplashScreen::SplashScreen() : Scene::Scene()
{
	stateScene = GameState::SPLASHSCREEN;
	timeOut = SPLASHSCREEN_TIME;
	Renderer::Instance()->LoadTexture("BG_SPLASH", SPLASHSCREEN_PATH); //posa la textura de la splashscreen en memoria
}

void SplashScreen::Update(InputStruct inputs) //update
{
	timeOut -= inputs.deltaTime;
	if (timeOut <= 0) stateScene = GameState::MENU; //quan s'acaba el temps passa a menu
}

void SplashScreen::Draw() { Renderer::Instance()->PushImage("BG_SPLASH", Rect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT }); } //draw

SplashScreen::~SplashScreen()
{
}

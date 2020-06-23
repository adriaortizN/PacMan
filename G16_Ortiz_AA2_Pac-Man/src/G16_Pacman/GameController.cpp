#include "GameController.h"
#include "Renderer.h"

GameController::GameController() : inputs( new Inputs()), currentScene(new SplashScreen())
{
	gameState = currentScene->getState(); //agafa l'estat de la escena
}

void GameController::GameLoop() { //gameloop

	while (gameState != GameState::EXIT) {
		Update();
		Draw();
	}

	delete Renderer::Instance();
}

void GameController::Update() {

	inputs->Update(); //input handler

	if (inputs->GetInputs().end) gameState = GameState::EXIT;
	else if (gameState != currentScene->getState()) { //iguala l'estat del game controller amb el de la escena si son diferents
		if (gameState == GameState::GAME && currentScene->getState() == GameState::RANKING) winner = dynamic_cast<Game*>(currentScene)->GetWinner();
		else  winner = { "NONE", 0 };
		gameState = currentScene->getState();

		switch (gameState)
		{
		case GameState::SPLASHSCREEN: 
			delete currentScene;
			currentScene = new SplashScreen(); //si l'estat es splashscreen elimina la escena actual i crea la splashscreen
			break;
		case GameState::MENU:
			delete currentScene;
			currentScene = new Menu(); //si l'estat es menu elimina la escena actual i crea la menu
			break;
		case GameState::GAME:
			delete currentScene;
			currentScene = new Game(); //si l'estat es game elimina la escena actual i crea la game
			break;
		case GameState::RANKING:
			//dynamic_cast<Game*>(currentScene)->GetWinner();
			delete currentScene;
			currentScene = new Ranking(winner);			//si l'estat es ranking elimina la escena actual i crea la ranking. Encara no esta fer				//currentScene = new Ranking(winner);	
			break;
		case GameState::EXIT:
			delete currentScene; //si l'estat es exit elimina la escena
			currentScene = nullptr;
			break;
		default:
			break;
		}
	}

	if(currentScene != nullptr)currentScene->Update(inputs->GetInputs()); //update de la escena actual
	//inputs->TestInputs(); //per testejar els inputs

}

void GameController::Draw() { //draw del gamecontroller
	Renderer* rend = Renderer::Instance();
	rend->Clear();
	if(currentScene != nullptr)currentScene->Draw();
	rend->Render();
}

GameController::~GameController()
{
}

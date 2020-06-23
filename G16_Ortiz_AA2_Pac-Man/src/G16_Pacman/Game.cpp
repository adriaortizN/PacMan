#include "Game.h"
#include "../../dep/inc/XML/rapidxml.hpp"
#include "../../dep/inc/XML/rapidxml_utils.hpp"
#include "../../dep/inc/XML/rapidxml_iterators.hpp"
#include "../../dep/inc/XML/rapidxml_print.hpp"
#include <sstream>
#include <time.h>


Game::Game() : title(new TextTexture{ {"Game Title Text", "Start Game", COLOR_WHITE}, {SCREEN_WIDTH/2, 100}, TITLE_FONT, ALIGN::CENTER }), //titol
sideTitle(new TextTexture{{"Game SubTitle Text", "SpaceBar to Start", COLOR_WHITE}, {SCREEN_WIDTH/2, 300}, DEFAULT_FONT, ALIGN::CENTER}), //subtitol
pauseTitle(new TextTexture{{"Game Pause Text", "...Pause...", COLOR_WHITE}, {SCREEN_WIDTH/2, 100}, TITLE_FONT, ALIGN::CENTER}), //titol de pausa
limits{ LIMITS_GAME }, soundButton(new Button("Sound On", Vector2{ SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 })), playerPos{ 0,0 }, BlinkyPos{ 0,0, }, InkyPos {0, 0}, ClydePos{ 0,0 }, PowerUpPos{ 0,0 }, 
playerLeftDown{0,0}, playerRightUp{0,0}, playerLeftUp{0,0}, playerRightDown{0,0}, centerPos{0,0}, ghostLeftUp{0,0}, ghostRightUp{0,0}, ghostLeftDown{0,0}, ghostRightDown{0,0}
{
	mixer = new Mixer(AUDIOPLAY_PATH, AUDIO_FREQUENCY);
	Renderer * rend = Renderer::Instance();
	rend->LoadTexture("BG_TRANS", TRANSPARENCY_PATH); //posa la textura del joc en memoria
	stateScene = GameState::GAME; //estat del joc
	gameSceneState = PlayGameSceneState::START_GAME; //estat a la escena
	LoadGrid(); //carrega el grid de blocs
	LoadHUD();

	cooldownFruit = 10.0;
	pointsToWin = 0;
	for (int i = 0; i < 3; i++) {
		ghostTimer[i] = 0.0;
		isRespawningGhost[i] = false;
	}
}

void Game::Update(InputStruct inputs) {
	
	switch (gameSceneState)
	{
	case START_GAME:
		if (inputs.inputsMap[keys[ACTIONS::ESCAPE]]) {
			std::cout << "MENU" << std::endl;
			stateScene = GameState::MENU; //si es prem la tecla de retornar, torna al menu		
		}
		else if (inputs.inputsMap[keys[ACTIONS::SPACEBAR]]) {
			std::cout << "RUNNING" << std::endl;
			gameSceneState = PlayGameSceneState::RUNNING; //si es prem la tecla de llançar la pilota comença la partida
		}
		else {
			gameSceneState = PlayGameSceneState::PAUSE;
		}
		break;
	case RUNNING:
		if (inputs.inputsMap[keys[ACTIONS::ESCAPE]] || inputs.inputsMap[keys[ACTIONS::P_KEY]]) gameSceneState = PlayGameSceneState::PAUSE; //si es prem la tecla de retornar o pausar es pausa la partida


		
		CheckPointsCollision();
		CheckFruitCollision();
		CheckGhostsCollision();
		CheckPowerUpsCollision();

		

		player->Update(inputs); //actualitza els inputs del jugador 1
		SetPlayerPos();

		GhostPowerUpsFunction();
		GhostUpdate();			//actualitza als fantasmes, amb les seves animacions 
		SetGhostPos();			//Moure els fantasmes 



		for (int i = 0; i < 3; i++) {
			if (ghost[i] != nullptr) {
				if (isRespawningGhost[i] == true) {
					RespawnGhost(i);
				}
			}
		}




		

		cooldownFruit = TimerFruit(inputs, cooldownFruit);
		SpawnFruit(cooldownFruit);


		break;
	case PAUSE:
		
		if (inputs.inputsMap[keys[ACTIONS::ESCAPE]] && !inputs.lastEsc) stateScene = GameState::MENU;		//si es prem la tecla de retornar al estat de pausa, torna al menu
		else if (inputs.inputsMap[keys[ACTIONS::SPACEBAR]]) gameSceneState = PlayGameSceneState::RUNNING;	//si es prem la tecla de llençar la bola el joc continua
		
		soundButton->Update(inputs);
		if (soundButton->IsPressed()) {
			bool isOff =  mixer->ToggleAudio();
			isOff ? soundButton->UpdateText("Sound Off") : soundButton->UpdateText("Sound On");
		}

		break;
	case GAME_OVER:
		winnerName = "Winner";
		std::cout << "Write your Name without Spaces" << std::endl;
		std::cin >> winnerName;
		winnerScore = player->GetScore();
		stateScene = GameState::RANKING;			//El ranking no es funcional, per tant, porta a menu
		break;
	default:
		break;
	}
	
}


void Game::Draw() {

	Renderer* rend = Renderer::Instance(); //crida al renderer
	rend->PushImage("BG_GENERAL", Rect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT }); //pinta el fons 

	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			if (gridWalls[i][j] != nullptr) {
				
				gridWalls[i][j]->Draw();		//pinta els Walls
			}
		}
	}

	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			if (gridPowerUps[i][j] != nullptr) {
				isPointEated = gridPowerUps[i][j]->GetIsEaten();	
				if (!isPointEated) {
					gridPowerUps[i][j]->Draw();	 //pinta els PowerUps que no s' han menjat
				}
			}
		}
	}



	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			if (gridPoints[i][j] != nullptr) {
				isPointEated = gridPoints[i][j]->GetIsEaten();
				if(!isPointEated){
				
					gridPoints[i][j]->Draw();	 //pinta els Points que no s' han menjat
				}
			}
		}
	}

	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 20; j++) {
			gridHUD[i][j]->Draw();				//pinta els blocs que no estan buits

		}
	}



	if (fruit != nullptr) {					//En cas d' haver alguna fruita spawnejada al mapa, es dibuixa
		fruit->Draw();
	}
	
	for (int i = 0; i < 3; i++) {
		if(ghost[i] != nullptr){					//En cas de no haver carregat algun fantasma, no es te d' intentar dibuixar, sino peta 
			if (isRespawningGhost[i] == false) {	//En cas de que el fantasma s' esta spawnejant
				ghost[i]->Draw();
			}
		}
	}


	player->Draw();//pinta al jugador 1
	
	

	
	switch (gameSceneState)
	{
	case START_GAME:
		rend->PushImage("BG_TRANS", Rect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT });
		title->Draw();							//pinta el text inicial
		sideTitle->Draw();						//pinta el subtitol
		break;
	case RUNNING:
		break;
	case PAUSE:
		rend->PushImage("BG_TRANS", Rect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT });
		pauseTitle->Draw();						//pinta el text de pausa
		soundButton->Draw();
		break;
	case GAME_OVER:
		break;
	default:
		break;
	}

}

void Game::LoadGrid() {						//carrega l'xml al joc
	srand(time(NULL));

	
	rapidxml::xml_document<> doc;
	std::ifstream file(XML_PATH);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	rapidxml::xml_node<> *pRoot = doc.first_node();
	rapidxml::xml_node<> *pRang = pRoot->first_node("Positions")->first_node("Player");					
	Vector2 tempPlayerPos { std::stoi(pRang->first_attribute("x")->value()), std::stoi(pRang->first_attribute("y")->value()) };
	playerPos = tempPlayerPos;
	playerPos.x -= 1;
	playerPos.y -= 1;
	player = new Player{ A, 0,	playerPos, new TextTexture{ { "Player Score", "0", COLOR_BLACK },{ 800, 100 }, DEFAULT_FONT, ALIGN::LEFT },
		new TextTexture{ { "Cherry Score", "0", COLOR_BLACK },{ 800, 225 }, DEFAULT_FONT, ALIGN::LEFT },
		new TextTexture{ { "Strawberry Score", "0", COLOR_BLACK },{ 800, 300 }, DEFAULT_FONT, ALIGN::LEFT },
		new TextTexture{ { "Orange Score", "0", COLOR_BLACK },{ 800, 375 }, DEFAULT_FONT, ALIGN::LEFT } };

	rapidxml::xml_node<> *pRang2 = pRang->next_sibling();												//Pasa a la seguente
	BlinkyPos = { std::stoi(pRang2->first_attribute("x")->value()), std::stoi(pRang2->first_attribute("y")->value()) };
	BlinkyPos.x -= 1;		//Donat que les coordenades están sobre cero, es te de restar 1 pk es coloquin on toqui
	BlinkyPos.y -= 1;
	ghost[0] = new Ghost{ AGHOST, 1, BlinkyPos, BLINKY };
	
	rapidxml::xml_node<> *pRang3 = pRang2->next_sibling();												//Pasa a la seguente

	InkyPos = { std::stoi(pRang3->first_attribute("x")->value()), std::stoi(pRang3->first_attribute("y")->value()) };
	InkyPos.x -= 1;			//Donat que les coordenades están sobre cero
	InkyPos.y -= 1;
	std::cout << "X: " << InkyPos.x << " Y: " << InkyPos.y << std::endl;
	ghost[1] = new Ghost{ AGHOST, 1, InkyPos, INKY };
	rapidxml::xml_node<> *pRang4 = pRang3->next_sibling();												//Pasa a la seguente

	ClydePos = { std::stoi(pRang4->first_attribute("x")->value()), std::stoi(pRang4->first_attribute("y")->value()) };
	ClydePos.x -= 1;		//Donat que les coordenades están sobre cero
	ClydePos.y -= 1;
	ghost[2] = new Ghost{ AGHOST, 2, ClydePos, CLYDE };

	

	rapidxml::xml_node<> *pPower = pRoot->first_node("Positions");
	for (rapidxml::xml_node<> *pNode = pPower->first_node("PowerUps")->first_node("Power"); pNode; pNode = pNode->next_sibling()) {

		int x = std::stoi(pNode->first_attribute("x")->value());
		int y = std::stoi(pNode->first_attribute("y")->value()); 

		if (x >= 0 && x < 21 && y >= 0 && y < 21) {
			gridPowerUps[x][y] = new PowerUp{ x,y };			//crea el Power Up
		}
		else if (x < 0 || y < 0) {
			gridPowerUps[x][y] = nullptr;
		}
		else {
			gridPowerUps[x][y] = new PowerUp{ x,y };			//No es tindria que crear cap Power Up mes 
		}
	}
	

	rapidxml::xml_node<> *pMap = pRoot->first_node("Map");
	for (rapidxml::xml_node<> *pNode = pMap->first_node("Wall"); pNode; pNode = pNode->next_sibling()) {
		int x = std::stoi(pNode->first_attribute("x")->value());
		int y = std::stoi(pNode->first_attribute("y")->value());
		if (x >= 0 && x < 21 && y >= 0 && y < 21) {
			gridWalls[x][y] = new Wall{ x,y };			//crea el bloc
		}
		else if (x < 0 || y < 0){
			gridWalls[x][y] = nullptr;
		}
		else {
			gridWalls[x][y] = new Wall{ x,y };			//No es tindria que crear cap wall mes 
		}

	}
	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			if (gridWalls[i][j] == nullptr) {
				if (gridPowerUps[i][j] == nullptr) {
						gridPoints[i][j] = new Point{ i,j };
						pointsInMap++;
				}
			}
		}
	}
	pointsInMap = pointsInMap - 41;			//41 son los puntos en los bordes del mapa

}

void Game::LoadHUD() {
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 20; j++) {
			if (gridHUD[i][j] == nullptr) {
				gridHUD[i][j] = new HUDBackground{ i,j };
			}
		}
	}
}


bool Game::CheckGridCollision(Axis _axis, Rect _playerVariables) {			//Collisions entre Player y Walls
	
	playerLeftUp.x = _playerVariables.x;
	playerLeftUp.y = _playerVariables.y;

	playerRightUp.x = _playerVariables.x + TEXT_SIZE - PACMAN_SPEED;
	playerRightUp.y = _playerVariables.y;

	playerLeftDown.x = _playerVariables.x;
	playerLeftDown.y = _playerVariables.y + TEXT_SIZE - PACMAN_SPEED;

	playerRightDown.x = _playerVariables.x + TEXT_SIZE - PACMAN_SPEED;
	playerRightDown.y = _playerVariables.y + TEXT_SIZE -PACMAN_SPEED;

	Vector2 firstCheck = { 0,0 };
	Vector2 secondCheck = { 0,0 };


	switch (_axis) {
	case GOUP:

		playerLeftUp.y -= PACMAN_SPEED;
		playerRightUp.y -= PACMAN_SPEED;

		firstCheck.x = ((playerLeftUp.x) / TEXT_SIZE) + 1;
		firstCheck.y = ((playerLeftUp.y) / TEXT_SIZE) + 1;

		secondCheck.x = ((playerRightUp.x) / TEXT_SIZE) + 1;
		secondCheck.y = ((playerRightUp.y) / TEXT_SIZE) + 1;
		
		if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
			
			return false;
		}
		else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
			return true;
		}
		
		else {
			centerPos = { (_playerVariables.x / TEXT_SIZE) + 1, (_playerVariables.y / TEXT_SIZE) + 1 };
			centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
			centerPos.y = ((centerPos.y - 1) * TEXT_SIZE);
			playerVariables->x = centerPos.x;
			playerVariables->y = centerPos.y;
			player->SetPlayerPos(*playerVariables);
			return true;
		}

		break;
	case GODOWN:

		playerLeftDown.y += PACMAN_SPEED;
		playerRightDown.y += PACMAN_SPEED;

		firstCheck.x = ((playerLeftDown.x) / TEXT_SIZE) + 1;
		firstCheck.y = ((playerLeftDown.y) / TEXT_SIZE) + 1;

		secondCheck.x = ((playerRightDown.x) / TEXT_SIZE) + 1;
		secondCheck.y = ((playerRightDown.y) / TEXT_SIZE) + 1;

		if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
			return false;
		}
		else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr){
			return true;
		}
		else {
			centerPos = { (_playerVariables.x / TEXT_SIZE) + 1, (_playerVariables.y / TEXT_SIZE) + 1 };
			centerPos.x = ((centerPos.x) * TEXT_SIZE);
			centerPos.y = ((centerPos.y - 1) * TEXT_SIZE);
			playerVariables->x = centerPos.x;
			playerVariables->y = centerPos.y;
			player->SetPlayerPos(*playerVariables);
			return true;
		}
		break;
	case GOLEFT:
		//std::cout << "GOLEFT" << std::endl;

		playerLeftUp.x -= PACMAN_SPEED;
		playerLeftDown.x -= PACMAN_SPEED;

		firstCheck.x = ((playerLeftUp.x) / TEXT_SIZE) + 1;
		firstCheck.y = ((playerLeftUp.y) / TEXT_SIZE) + 1;

		secondCheck.x = ((playerLeftDown.x) / TEXT_SIZE) + 1;
		secondCheck.y = ((playerLeftDown.y) / TEXT_SIZE) + 1;

		if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
			return false;
		}
		else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr){
			return true;
		}
		else {
			centerPos = { (_playerVariables.x / TEXT_SIZE) + 1, (_playerVariables.y / TEXT_SIZE) + 1 };
			centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
			centerPos.y = ((centerPos.y) * TEXT_SIZE);
			playerVariables->x = centerPos.x;
			playerVariables->y = centerPos.y;
			player->SetPlayerPos(*playerVariables);
			return true;
		}

		break;	
	case GORIGHT:

		playerRightUp.x += PACMAN_SPEED;
		playerRightDown.x += PACMAN_SPEED;

		firstCheck.x = ((playerRightUp.x) / TEXT_SIZE) + 1;
		firstCheck.y = ((playerRightUp.y) / TEXT_SIZE) + 1;

		secondCheck.x = ((playerRightDown.x) / TEXT_SIZE) + 1;
		secondCheck.y = ((playerRightDown.y) / TEXT_SIZE) + 1;

		if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
			return false;
		}
		else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr){
			return true;
		}

		else {
			centerPos = { (_playerVariables.x / TEXT_SIZE) + 1, (_playerVariables.y / TEXT_SIZE) + 1 };
			centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
			centerPos.y = ((centerPos.y) * TEXT_SIZE);
			playerVariables->x = centerPos.x;
			playerVariables->y = centerPos.y;
			player->SetPlayerPos(*playerVariables);
			return true;
		}

		break;
	}
	
}
	
	

void Game::SetPlayerPos() {

	
	playerVariables = &player->GetPlayerPos();
	tempPlayerVariables = &player->GetPlayerPos();
	
	switch (player->direction) {
	case W:
		axis = Axis::GOUP;
		if (!CheckGridCollision(axis, *playerVariables)) {
			
			playerVariables->y -= PACMAN_SPEED;
		}else {												//Ha hagut colisio
			player->direction = STOP;
		}
		break;
		
	case A:
		axis = Axis::GOLEFT;
			if(!CheckGridCollision(axis, *playerVariables)){
				
				playerVariables->x -= PACMAN_SPEED;
			}
			else {												//Ha hagut colisio
				player->direction = STOP;
			}
		
		break;
	case S:
		axis = Axis::GODOWN;
			if (!CheckGridCollision(axis, *playerVariables)) {
				
				playerVariables->y += PACMAN_SPEED;
			}else {												//Ha hagut colisio
				player->direction = STOP;
			}
		break;
	case D:
		axis = Axis::GORIGHT;
			if (!CheckGridCollision(axis, *playerVariables)) {
				
				playerVariables->x += PACMAN_SPEED;
			}else {												//Ha hagut colisio
				player->direction = STOP;
			}

		break;
	case STOP:

		break;
	}
	
	player->SetPlayerPos(*playerVariables);


}

void Game::ReinitVariablesGhost() {										//Reinicia los valores de las variables
	ghostLeftUp.x = ghostVariables->x;
	ghostLeftUp.y = ghostVariables->y;

	ghostRightUp.x = ghostVariables->x + TEXT_SIZE - PACMAN_SPEED;
	ghostRightUp.y = ghostVariables->y;

	ghostLeftDown.x = ghostVariables->x;
	ghostLeftDown.y = ghostVariables->y + TEXT_SIZE - PACMAN_SPEED;

	ghostRightDown.x = ghostVariables->x + TEXT_SIZE - PACMAN_SPEED;
	ghostRightDown.y = ghostVariables->y + TEXT_SIZE - PACMAN_SPEED;
}

void Game::SetGhostPos() {										//Moviment dels fantasmes i "colisions" amb walls 
	for (int i = 0; i < 3; i++) {
		if (ghost[i] != nullptr) {
			ghostVariables = &ghost[i]->GetGhostPos();

			ReinitVariablesGhost();

			Vector2 firstCheck = { 0,0 };
			Vector2 secondCheck = { 0,0 };

			
			XCollisions = ((ghostVariables->x) / TEXT_SIZE) + 1;
			YCollisions = ((ghostVariables->y) / TEXT_SIZE) + 1;
			
			
			Rect temp = { 0, 0, 0, 0 };

			ghostType = ghost[i]->GetGhostType();

			int n = 0;
			
			switch (ghostType) {
			case BLINKY:

				ghostDirections = ghost[i]->GetDirectionGhost();

				

				switch (ghostDirections) {

				case WGHOST:
					//Calcular si hi ha un mur cap a dalt (GOUP)
					//Si hi ha mur, ficar a posibles direccions el WGHOST i n++ 

					ghostLeftUp.y -= PACMAN_SPEED;
					ghostRightUp.y -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = WGHOST;
						n++;
					}

					ReinitVariablesGhost();

					//Calcular si hi ha un mur cap a dreta (GORIGHT)
					//Si hi ha mur, ficar a posibles direccions el DGHOST i n++ 

					ghostRightUp.x += PACMAN_SPEED;
					ghostRightDown.x += PACMAN_SPEED;

					firstCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = DGHOST;
						n++;
					}

					ReinitVariablesGhost();

					//Calcular si hi ha un mur cap a esquerra (GOLEFT)
					//Si hi ha mur, ficar a posibles direccions el AGHOST i n++ 

					ghostLeftUp.x -= PACMAN_SPEED;
					ghostLeftDown.x -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = AGHOST;
						n++;
					}

					MoveBlinkyGhost(ghostPosiblesDirections, n, i);
					n = 0;

					ReinitVariablesGhost();

				break;


				case DGHOST:

					//Mur WGHOST

					ghostLeftUp.y -= PACMAN_SPEED;
					ghostRightUp.y -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = WGHOST;
						n++;
					}

					ReinitVariablesGhost();

					//Mur DGHOST

					ghostRightUp.x += PACMAN_SPEED;
					ghostRightDown.x += PACMAN_SPEED;

					firstCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = DGHOST;
						n++;
					}

					ReinitVariablesGhost();

					//Mur SGHOST

					ghostLeftDown.y += PACMAN_SPEED;
					ghostRightDown.y += PACMAN_SPEED;

					firstCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = SGHOST;
						n++;
					}
					
					MoveBlinkyGhost(ghostPosiblesDirections, n, i);
					n = 0;

					ReinitVariablesGhost();

					break;

				case AGHOST:

					ghostLeftUp.y -= PACMAN_SPEED;
					ghostRightUp.y -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = WGHOST;
						n++;
					}

					ReinitVariablesGhost();


					ghostLeftUp.x -= PACMAN_SPEED;
					ghostLeftDown.x -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = AGHOST;
						n++;
					}

					ReinitVariablesGhost();

					ghostLeftDown.y += PACMAN_SPEED;
					ghostRightDown.y += PACMAN_SPEED;

					firstCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = SGHOST;
						n++;
					}

					MoveBlinkyGhost(ghostPosiblesDirections, n, i);
					n = 0;

					ReinitVariablesGhost();

					break;

				case SGHOST:

					ghostLeftUp.x -= PACMAN_SPEED;
					ghostLeftDown.x -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = AGHOST;
						n++;
					}

					ReinitVariablesGhost();

					ghostRightUp.x += PACMAN_SPEED;
					ghostRightDown.x += PACMAN_SPEED;

					firstCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = DGHOST;
						n++;
					}

					ReinitVariablesGhost();

					ghostLeftDown.y += PACMAN_SPEED;
					ghostRightDown.y += PACMAN_SPEED;

					firstCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						ghostPosiblesDirections[n] = SGHOST;
						n++;
					}

					MoveBlinkyGhost(ghostPosiblesDirections, n, i);
					n = 0;

					ReinitVariablesGhost();

					break;
				}
				break;

			case INKY:											//Mateix moviment que el player 
				switch (player->direction) {
				case W:
					ghostLeftUp.y -= PACMAN_SPEED;
					ghostRightUp.y -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {

						//Puede moverse
						if (ghostIsVulnerable) {
							ghostVariables->y -= PACMAN_SPEED / 2;
						}
						else {
							ghostVariables->y -= PACMAN_SPEED;
						}

						ghost[i]->SetDirectionGhost(WGHOST);
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
						//No pot moures
					}

					else {
						centerPos = { (ghostVariables->x / TEXT_SIZE) + 1, (ghostVariables->y / TEXT_SIZE) + 1 };
						centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
						centerPos.y = ((centerPos.y - 1) * TEXT_SIZE);
						ghostVariables->x = centerPos.x;
						ghostVariables->y = centerPos.y;
						ghost[i]->SetGhostPos(*ghostVariables);
						//No pot moures
					}
					break;


				case D:

					ghostRightUp.x += PACMAN_SPEED;
					ghostRightDown.x += PACMAN_SPEED;

					firstCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						if (ghostIsVulnerable) {
							ghostVariables->x += PACMAN_SPEED / 2;
						}
						else {
							ghostVariables->x += PACMAN_SPEED;
						}

						ghost[i]->SetDirectionGhost(DGHOST);
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
						//No es mou
					}

					else {
						centerPos = { (ghostVariables->x / TEXT_SIZE) + 1, (ghostVariables->y / TEXT_SIZE) + 1 };
						centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
						centerPos.y = ((centerPos.y) * TEXT_SIZE);
						ghostVariables->x = centerPos.x;
						ghostVariables->y = centerPos.y;
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					break;

				case A:


					ghostLeftUp.x -= PACMAN_SPEED;
					ghostLeftDown.x -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						if (ghostIsVulnerable) {
							ghostVariables->x -= PACMAN_SPEED / 2;
						}
						else {
							ghostVariables->x -= PACMAN_SPEED;
						}

						ghost[i]->SetDirectionGhost(AGHOST);
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
						//No es mou
					}
					else {
						centerPos = { (ghostVariables->x / TEXT_SIZE) + 1, (ghostVariables->y / TEXT_SIZE) + 1 };
						centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
						centerPos.y = ((centerPos.y) * TEXT_SIZE);
						ghostVariables->x = centerPos.x;
						ghostVariables->y = centerPos.y;
						ghost[i]->SetGhostPos(*ghostVariables);
					}

					break;

				case S:

					ghostLeftDown.y += PACMAN_SPEED;
					ghostRightDown.y += PACMAN_SPEED;

					firstCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						if (ghostIsVulnerable) {
							ghostVariables->y += PACMAN_SPEED / 2;
						}
						else {
							ghostVariables->y += PACMAN_SPEED;
						}

						ghost[i]->SetDirectionGhost(SGHOST);
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
						//No es mou
					}
					else {
						centerPos = { (ghostVariables->x / TEXT_SIZE) + 1, (ghostVariables->y / TEXT_SIZE) + 1 };
						centerPos.x = ((centerPos.x) * TEXT_SIZE);
						centerPos.y = ((centerPos.y - 1) * TEXT_SIZE);
						ghostVariables->x = centerPos.x;
						ghostVariables->y = centerPos.y;
						ghost[i]->SetGhostPos(*ghostVariables);
					}

					break;
				}
				break;

			case CLYDE:
				switch (player->direction) {
				case S:
					ghostLeftUp.y -= PACMAN_SPEED;
					ghostRightUp.y -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {

						//Puede moverse
						if (ghostIsVulnerable) {
							ghostVariables->y -= PACMAN_SPEED / 2;
						}
						else {
							ghostVariables->y -= PACMAN_SPEED;
						}

						ghost[i]->SetDirectionGhost(WGHOST);
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
						//No pot moures
					}

					else {
						centerPos = { (ghostVariables->x / TEXT_SIZE) + 1, (ghostVariables->y / TEXT_SIZE) + 1 };
						centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
						centerPos.y = ((centerPos.y - 1) * TEXT_SIZE);
						ghostVariables->x = centerPos.x;
						ghostVariables->y = centerPos.y;
						ghost[i]->SetGhostPos(*ghostVariables);
						//No pot moures
					}
					break;

				case A:
					ghostRightUp.x += PACMAN_SPEED;
					ghostRightDown.x += PACMAN_SPEED;

					firstCheck.x = ((ghostRightUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostRightUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						if (ghostIsVulnerable) {
							ghostVariables->x += PACMAN_SPEED / 2;
						}
						else {
							ghostVariables->x += PACMAN_SPEED;
						}

						ghost[i]->SetDirectionGhost(DGHOST);
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
						//No es mou
					}

					else {
						centerPos = { (ghostVariables->x / TEXT_SIZE) + 1, (ghostVariables->y / TEXT_SIZE) + 1 };
						centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
						centerPos.y = ((centerPos.y) * TEXT_SIZE);
						ghostVariables->x = centerPos.x;
						ghostVariables->y = centerPos.y;
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					break;

				case D:
					ghostLeftUp.x -= PACMAN_SPEED;
					ghostLeftDown.x -= PACMAN_SPEED;

					firstCheck.x = ((ghostLeftUp.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftUp.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						if (ghostIsVulnerable) {
							ghostVariables->x -= PACMAN_SPEED / 2;
						}
						else {
							ghostVariables->x -= PACMAN_SPEED;
						}

						ghost[i]->SetDirectionGhost(AGHOST);
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
						//No es mou
					}
					else {
						centerPos = { (ghostVariables->x / TEXT_SIZE) + 1, (ghostVariables->y / TEXT_SIZE) + 1 };
						centerPos.x = ((centerPos.x - 1) * TEXT_SIZE);
						centerPos.y = ((centerPos.y) * TEXT_SIZE);
						ghostVariables->x = centerPos.x;
						ghostVariables->y = centerPos.y;
						ghost[i]->SetGhostPos(*ghostVariables);
					}

					break;

				case W:
					ghostLeftDown.y += PACMAN_SPEED;
					ghostRightDown.y += PACMAN_SPEED;

					firstCheck.x = ((ghostLeftDown.x) / TEXT_SIZE) + 1;
					firstCheck.y = ((ghostLeftDown.y) / TEXT_SIZE) + 1;

					secondCheck.x = ((ghostRightDown.x) / TEXT_SIZE) + 1;
					secondCheck.y = ((ghostRightDown.y) / TEXT_SIZE) + 1;

					if (gridWalls[firstCheck.x][firstCheck.y] == nullptr && gridWalls[secondCheck.x][secondCheck.y] == nullptr) {
						if (ghostIsVulnerable) {
							ghostVariables->y += PACMAN_SPEED / 2;
						}
						else {
							ghostVariables->y += PACMAN_SPEED;
						}

						ghost[i]->SetDirectionGhost(SGHOST);
						ghost[i]->SetGhostPos(*ghostVariables);
					}
					else if (gridWalls[firstCheck.x][firstCheck.y] != nullptr && gridWalls[secondCheck.x][secondCheck.y] != nullptr) {
						//No es mou
					}
					else {
						centerPos = { (ghostVariables->x / TEXT_SIZE) + 1, (ghostVariables->y / TEXT_SIZE) + 1 };
						centerPos.x = ((centerPos.x) * TEXT_SIZE);
						centerPos.y = ((centerPos.y - 1) * TEXT_SIZE);
						ghostVariables->x = centerPos.x;
						ghostVariables->y = centerPos.y;
						ghost[i]->SetGhostPos(*ghostVariables);
					}

					break;
				}
				break;
			}
		}
	}
}

void Game::MoveBlinkyGhost(GhostDirection _directions[], int size, int _i) {

	int range = size - 0 + 1;
	int num = rand() % range + 0;

	//std::cout << num << std::endl;
	ghostDirections = _directions[num];

	//std::cout << ghostDirections << std::endl;

	switch (ghostDirections) {
	case AGHOST:
			if (ghostIsVulnerable) {
				ghostVariables->x -= PACMAN_SPEED / 2;
			}
			else {
				ghostVariables->x -= PACMAN_SPEED;
			}

			ghost[_i]->SetDirectionGhost(AGHOST);
			ghost[_i]->SetGhostPos(*ghostVariables);
		break;

		case WGHOST:
			if (ghostIsVulnerable) {
				ghostVariables->y -= PACMAN_SPEED / 2;
			}
			else {
				ghostVariables->y -= PACMAN_SPEED;
			}

			ghost[_i]->SetDirectionGhost(WGHOST);
			ghost[_i]->SetGhostPos(*ghostVariables);
		break;

	case DGHOST:
			if (ghostIsVulnerable) {
				ghostVariables->x += PACMAN_SPEED / 2;
			}
			else {
				ghostVariables->x += PACMAN_SPEED;
			}

			ghost[_i]->SetDirectionGhost(DGHOST);
			ghost[_i]->SetGhostPos(*ghostVariables);
		break;

	case SGHOST:
			if (ghostIsVulnerable) {
				ghostVariables->y += PACMAN_SPEED / 2;
			}
			else {
				ghostVariables->y += PACMAN_SPEED;
			}

			ghost[_i]->SetDirectionGhost(SGHOST);
			ghost[_i]->SetGhostPos(*ghostVariables);
		break;

	}
	
}

void Game::CheckPointsCollision() {						//Colisio entre player y Points

	playerVariables = &player->GetPlayerPos();

	playerLeftUp.x = playerVariables->x;
	playerLeftUp.y = playerVariables->y;

	playerRightUp.x = playerVariables->x + TEXT_SIZE - PACMAN_SPEED;
	playerRightUp.y = playerVariables->y;

	playerLeftDown.x = playerVariables->x;
	playerLeftDown.y = playerVariables->y + TEXT_SIZE - PACMAN_SPEED;

	playerRightDown.x = playerVariables->x + TEXT_SIZE - PACMAN_SPEED;
	playerRightDown.y = playerVariables->y + TEXT_SIZE - PACMAN_SPEED;

	Vector2 firstCheck = { 0,0 };
	Vector2 secondCheck = { 0,0 };

	switch (axis) {
	case GOUP:
		//std::cout << "GOUP" << std::endl;

		playerLeftUp.y -= PACMAN_SPEED;
		playerRightUp.y -= PACMAN_SPEED;

		firstCheck.x = ((playerLeftUp.x) / TEXT_SIZE) + 1;
		firstCheck.y = ((playerLeftUp.y) / TEXT_SIZE) + 1;

		secondCheck.x = ((playerRightUp.x) / TEXT_SIZE) + 1;
		secondCheck.y = ((playerRightUp.y) / TEXT_SIZE) + 1;

		if (gridPoints[firstCheck.x][firstCheck.y] == nullptr && gridPoints[secondCheck.x][secondCheck.y] == nullptr) {

			
		}
		else if (gridPoints[firstCheck.x][firstCheck.y] != nullptr) {
			isPointEated = gridPoints[firstCheck.x][firstCheck.y]->GetIsEaten();
			if (!isPointEated) {
				gridPoints[firstCheck.x][firstCheck.y]->Eat();
				player->addScore(1);
				pointsToWin++;
				if (pointsToWin >= pointsInMap) {
					gameSceneState = PlayGameSceneState::GAME_OVER;
				}
			}
		}
		else if (gridPoints[secondCheck.x][secondCheck.y] != nullptr) {
			isPointEated = gridPoints[secondCheck.x][secondCheck.y]->GetIsEaten();
			if (!isPointEated) {
				gridPoints[secondCheck.x][secondCheck.y]->Eat();
				player->addScore(1);
				pointsToWin++;
				if (pointsToWin >= pointsInMap) {
					gameSceneState = PlayGameSceneState::GAME_OVER;
				}
			}
		}

		break;
	case GODOWN:
		//std::cout << "GODOWN" << std::endl;

		playerLeftDown.y += PACMAN_SPEED;
		playerRightDown.y += PACMAN_SPEED;

		firstCheck.x = ((playerLeftDown.x) / TEXT_SIZE) + 1;
		firstCheck.y = ((playerLeftDown.y) / TEXT_SIZE) + 1;

		secondCheck.x = ((playerRightDown.x) / TEXT_SIZE) + 1;
		secondCheck.y = ((playerRightDown.y) / TEXT_SIZE) + 1;

		if (gridPoints[firstCheck.x][firstCheck.y] == nullptr && gridPoints[secondCheck.x][secondCheck.y] == nullptr) {
			
		}
		else if (gridPoints[firstCheck.x][firstCheck.y] != nullptr) {
			isPointEated = gridPoints[firstCheck.x][firstCheck.y]->GetIsEaten();
			if (!isPointEated) {
				gridPoints[firstCheck.x][firstCheck.y]->Eat();
				player->addScore(1);
				pointsToWin++;
				if (pointsToWin >= pointsInMap) {
					gameSceneState = PlayGameSceneState::GAME_OVER;
				}
			}
		}
		else if (gridPoints[secondCheck.x][secondCheck.y] != nullptr) {
			isPointEated = gridPoints[secondCheck.x][secondCheck.y]->GetIsEaten();
			if (!isPointEated) {
				gridPoints[secondCheck.x][secondCheck.y]->Eat();
				player->addScore(1);
				pointsToWin++;
				if (pointsToWin >= pointsInMap) {
					gameSceneState = PlayGameSceneState::GAME_OVER;
				}
			}
		}
		break;
	case GOLEFT:
		//std::cout << "GOLEFT" << std::endl;

		playerLeftUp.x -= PACMAN_SPEED;
		playerLeftDown.x -= PACMAN_SPEED;

		firstCheck.x = ((playerLeftUp.x) / TEXT_SIZE) + 1;
		firstCheck.y = ((playerLeftUp.y) / TEXT_SIZE) + 1;

		secondCheck.x = ((playerLeftDown.x) / TEXT_SIZE) + 1;
		secondCheck.y = ((playerLeftDown.y) / TEXT_SIZE) + 1;

		if (gridPoints[firstCheck.x][firstCheck.y] == nullptr && gridPoints[secondCheck.x][secondCheck.y] == nullptr) {
			
		}
		else if (gridPoints[firstCheck.x][firstCheck.y] != nullptr) {
			isPointEated = gridPoints[firstCheck.x][firstCheck.y]->GetIsEaten();
			if (!isPointEated) {
				gridPoints[firstCheck.x][firstCheck.y]->Eat();
				player->addScore(1);
				pointsToWin++;
				if (pointsToWin >= pointsInMap) {
					gameSceneState = PlayGameSceneState::GAME_OVER;
				}
			}
		}
		else if (gridPoints[secondCheck.x][secondCheck.y] != nullptr) {
			isPointEated = gridPoints[secondCheck.x][secondCheck.y]->GetIsEaten();
			if (!isPointEated) {
				gridPoints[secondCheck.x][secondCheck.y]->Eat();
				player->addScore(1);
				pointsToWin++;
				if (pointsToWin >= pointsInMap) {
					gameSceneState = PlayGameSceneState::GAME_OVER;
				}
			}
		}
		break;
	case GORIGHT:
		//std::cout << "GORIGHT" << std::endl;

		playerRightUp.x += PACMAN_SPEED;
		playerRightDown.x += PACMAN_SPEED;

		firstCheck.x = ((playerRightUp.x) / TEXT_SIZE) + 1;
		firstCheck.y = ((playerRightUp.y) / TEXT_SIZE) + 1;

		secondCheck.x = ((playerRightDown.x) / TEXT_SIZE) + 1;
		secondCheck.y = ((playerRightDown.y) / TEXT_SIZE) + 1;

		if (gridPoints[firstCheck.x][firstCheck.y] == nullptr && gridPoints[secondCheck.x][secondCheck.y] == nullptr) {
			
		}
		else if (gridPoints[firstCheck.x][firstCheck.y] != nullptr) {
			isPointEated = gridPoints[firstCheck.x][firstCheck.y]->GetIsEaten();
			if (!isPointEated) {
				gridPoints[firstCheck.x][firstCheck.y]->Eat();
				player->addScore(1);
				pointsToWin++;
				if (pointsToWin >= pointsInMap) {
					gameSceneState = PlayGameSceneState::GAME_OVER;
				}
			}
		}
		else if (gridPoints[secondCheck.x][secondCheck.y] != nullptr) {
			isPointEated = gridPoints[secondCheck.x][secondCheck.y]->GetIsEaten();
			if (!isPointEated) {
				gridPoints[secondCheck.x][secondCheck.y]->Eat();
				player->addScore(1);
				pointsToWin++;
				if (pointsToWin >= pointsInMap) {
					gameSceneState = PlayGameSceneState::GAME_OVER;
				}
			}
		}

		break;
	}
}

void Game::CheckFruitCollision() {						//Colisio entre player y Fruit
	if (fruit != nullptr) {
		playerVariables = &player->GetPlayerPos();
		fruitVariables = &fruit->GetPosition();
		if (playerVariables->x - 5 < fruitVariables->x + fruitVariables->w &&
			playerVariables->x + fruitVariables->w - 5 > fruitVariables->x &&
			playerVariables->y < fruitVariables->y + fruitVariables->h &&
			playerVariables->h + playerVariables->y > fruitVariables->y)
		{
			//Hi ha colisio
			incomingPoints = fruit->GetPoints();
			player->addScore(incomingPoints);

			
			fruitType = fruit->GetFruitType();

			switch (fruitType) {
			case CHERRY:
				player->addCherry();
				break;
			case STRAWBERRY:
				player->addStrawberry();
				break;
			case ORANGE:
				player->addOrange();
				break;
			}

			cooldownFruit = 15.0;				//Es fica el temporitzador a 15 segons
			
			fruit = nullptr;
			
		}
	}
}

void Game::CheckGhostsCollision() {						//Colisio entre player y Ghost
	for (int i = 0; i < 3; i++) {
		if (ghost[i] != nullptr) {
			playerVariables = &player->GetPlayerPos();
			ghostVariables = &ghost[i]->GetGhostPos();
			if (playerVariables->x - 5 < ghostVariables->x + ghostVariables->w &&
				playerVariables->x + ghostVariables->w - 5 > ghostVariables->x &&
				playerVariables->y < ghostVariables->y + ghostVariables->h &&
				playerVariables->h + playerVariables->y > ghostVariables->y) {
				ghostIsVulnerable = ghost[i]->GetGhostIsVulnerable();
				if (!ghostIsVulnerable) {			//Jugador ha chocat contra ghost i aquest NO estava vulnerable 
					

					RespawnPlayer();
					
				}
				else {								//Jugador ha chocat contra ghost i aquest estava vulnerable 
					isRespawningGhost[i] = true;

					ghostType = ghost[i]->GetGhostType();

					switch (ghostType) {
					case BLINKY:
						player->addScore(25);
						break;
					case INKY:
						player->addScore(10);
						break;

					case CLYDE:
						player->addScore(15);
						break;

					}
				}

			}
		}
	}
}

void Game::CheckPowerUpsCollision() {					//Collisio entre player y PowerUps
	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			if (gridPowerUps[i][j] != nullptr) {

				playerVariables = &player->GetPlayerPos();
				powerUpsVariables = &gridPowerUps[i][j]->GetPosition();

				isPointEated = gridPowerUps[i][j]->GetIsEaten();

				if (!isPointEated) {
					if (playerVariables->x - 5< powerUpsVariables->x + powerUpsVariables->w &&
						playerVariables->x + playerVariables->w - 5 > powerUpsVariables->x &&
						playerVariables->y < powerUpsVariables->y + powerUpsVariables->h &&
						playerVariables->h + playerVariables->y > powerUpsVariables->y) {
						// ¡colision detectada!


						gridPowerUps[i][j]->Eat();

						ghostIsVulnerable = true;
						powerUpsTime = 10.0;
						for (int i = 0; i < 3; i++) {
							if (ghost[i] != nullptr) {
								ghost[i]->SwitchGhostIsVulnerable(true);
							}
						}

					}
				}
			}
		}
	}
}

void Game::GhostUpdate() {
	for (int i = 0; i < 3; i++) {
		if (ghost[i] != nullptr) {
			ghostDirections = ghost[i]->GetDirectionGhost();
			ghost[i]->Update(ghostDirections);
		}
	}
}

void Game::RespawnPlayer() {
	SDL_Delay(2000);							//Espera 2 segons a fer spawn;
	playerVariables->x = playerPos.x * TEXT_SIZE;
	playerVariables->y = playerPos.y * TEXT_SIZE;
	player->TransportPlayer(*playerVariables);
	if (player->GetLifes() > 1) {

		player->ReduceLife();

		

		for (int i = 0; i < 3; i++) {

			if(ghost[i] != nullptr){

				ghostType = ghost[i]->GetGhostType();

				switch (ghostType) {

				case BLINKY:
					ghostVariables->x = BlinkyPos.x * TEXT_SIZE;
					ghostVariables->y = BlinkyPos.y * TEXT_SIZE;
					
					ghost[i]->SetGhostPos(*ghostVariables);
					break;
				case INKY:
					ghostVariables->x = InkyPos.x * TEXT_SIZE;
					ghostVariables->y = InkyPos.y * TEXT_SIZE;
					
					ghost[i]->SetGhostPos(*ghostVariables);
					break;

				case CLYDE:
					ghostVariables->x = ClydePos.x * TEXT_SIZE;
					ghostVariables->y = ClydePos.y * TEXT_SIZE;
					
					ghost[i]->SetGhostPos(*ghostVariables);
					break;

				}
			}
		}
	}
	else {
		//GAME OVER
		gameSceneState = GAME_OVER;			
	}


	
}

void Game::RespawnGhost(int n) {

	for (int i = 0; i < 3; i++) {

		if(i == n){

			if (ghostTimer[i] < 2.0) {							//El temps de dos segons encara no ha pasat
				ghostVariables->x = 9999999;					//Posisio molt lluny 
				ghostVariables->y = 9999999;
				ghost[i]->SetGhostPos(*ghostVariables);

				ghostTimer[i] += FRAME_TIME;						//Aumentar el temps 
			}else{												//Si el temps ja ha pasat
				ghostType = ghost[i]->GetGhostType();

				isRespawningGhost[i] = false;
				ghostTimer[i] = 0.0;

				switch (ghostType) {

				case BLINKY:
						ghostVariables->x = BlinkyPos.x * TEXT_SIZE;
						ghostVariables->y = BlinkyPos.y * TEXT_SIZE;
						ghost[i]->SetGhostPos(*ghostVariables);
					break;

				case INKY:
					ghostVariables->x = InkyPos.x * TEXT_SIZE;
					ghostVariables->y = InkyPos.y * TEXT_SIZE;
					ghost[i]->SetGhostPos(*ghostVariables);
					break;

				case CLYDE:
					ghostVariables->x = ClydePos.x * TEXT_SIZE;
					ghostVariables->y = ClydePos.y * TEXT_SIZE;
					ghost[i]->SetGhostPos(*ghostVariables);
					break;


				}
			}
		}

	}
}



void Game::GhostPowerUpsFunction() {						//Gestio dels power ups 
	if (ghostIsVulnerable) {								//Si el powerUp esta actiu
		if (powerUpsTime <= 0.0) {							//Si el temps es inferior a 0 (Han pasat 10 segons), el powerUp ha acabat
			powerUpsTime = 10.0;							//Resetejar el temps
			ghostIsVulnerable = false;					

			for (int i = 0; i < 3; i++) {					//Recorro tots els fantasmes
				if (ghost[i] != nullptr) {
					ghost[i]->SwitchGhostIsVulnerable(false);	//Els fantasmes no son vulnerables
				}
			}
			
		}
		else {												//El powerUp segeix estant actiu
			powerUpsTime -= FRAME_TIME;						//
			
		}
		
	}
	else {
		
	}
}

	

float Game::TimerFruit(InputStruct inputs, float _cooldownFruit) {
	_cooldownFruit -= inputs.deltaTime;
	return _cooldownFruit;
}

void Game::SpawnFruit(float _cooldownFruit) {
	
	if (_cooldownFruit <= 0.0) {
		//Spawn de la fruit 
		random = rand() % 100;				//Random int entre 0 y 99
		if (random <= 33 && previousFruitType != CHERRY) {
			//Spawneja cherry 
			fruit = new Fruit(CHERRY, playerPos);
			previousFruitType = CHERRY;
			cooldownFruit = 999999999999999;	//Cooldown molt llarg pk no faci spawn d' una altra fruita  
		}
		else if (random <= 66 && random >= 34 && previousFruitType != STRAWBERRY) {
			//Spawneja strawberry
			fruit = new Fruit(STRAWBERRY, playerPos);
			previousFruitType = STRAWBERRY;
			cooldownFruit = 999999999999999;	//Cooldown molt llarg pk no faci spawn d' una altra fruita  
		}
		else if (random >= 67 && previousFruitType != ORANGE){
			//Spawneja orange
			fruit = new Fruit(ORANGE, playerPos);
			previousFruitType = ORANGE;
			cooldownFruit = 999999999999999;	//Cooldown molt llarg pk no faci spawn d' una altra fruita  
		}
		else {
			SpawnFruit(_cooldownFruit);
		}
		
	}
}


void Game::TestState() {
	std::cout<< "State: " << gameSceneState << std::endl;		//per testejar el estat en el que es troba
}

bool Game::CheckLimits(Vector2 rect) {
	return !(rect.x < limits.x + limits.w && rect.x > limits.x); //comprova si el vector esta dintre dels limits
}

std::pair<std::string, int> Game::GetWinner() {
	if(winnerName == "NONE") return std::pair<std::string, int>{"NONE", 0};
	return std::pair<std::string, int>{winnerName, winnerScore};
}

Game::~Game() //destructor
{
}

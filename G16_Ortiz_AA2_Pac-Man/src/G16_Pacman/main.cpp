#include "GameController.h"
//10.0.16299.0 en casa
//10.0.17134.0 en el resto
int main(int, char*[]) {	//main

	GameController* game = new GameController();

	game->GameLoop();

	return 0;
}
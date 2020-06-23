#include "Inputs.h"
#include <iostream>



Inputs::Inputs() : allInputs( //inicialitza totes les keys que s'utilitzaran i altres facilitats
	{ {keys[ACTIONS::W_KEY], false}, { keys[ACTIONS::A_KEY], false },{ keys[ACTIONS::S_KEY], false },{ keys[ACTIONS::D_KEY], false }, 
	{ keys[ACTIONS::UP_KEY], false }, { keys[ACTIONS::DOWN_KEY], false },
	{ keys[ACTIONS::SPACEBAR], false }, { keys[ACTIONS::ESCAPE], false }, {keys[ACTIONS::P_KEY], false} },
	false, false, { 0,0 }, false){}

void Inputs::Update() {

	UpdateTime();
	std::map<SDL_Keycode, bool>::iterator it;
	allInputs.lastMouse = allInputs.mouse;
	allInputs.lastEsc = allInputs.inputsMap[keys[ACTIONS::ESCAPE]];
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//Inputs::ResetKeys(inputs);
		switch (event.type) {
		case SDL_QUIT:
			allInputs.end = true;
			break;

		case SDL_KEYDOWN:
			it = allInputs.inputsMap.find(event.key.keysym.sym); //busca si existeix la key que s'ha presionat i si existeix la posa a true
			if (it != allInputs.inputsMap.end()) allInputs.inputsMap[event.key.keysym.sym] = true;
			break;

		case SDL_KEYUP:
			it = allInputs.inputsMap.find(event.key.keysym.sym);//busca si existeix la key que s'ha presionat i si existeix la posa a false
			if (it != allInputs.inputsMap.end()) allInputs.inputsMap[event.key.keysym.sym] = false;
			break;

		case SDL_MOUSEMOTION:
			allInputs.mousePos.x = event.motion.x;
			allInputs.mousePos.y = event.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			allInputs.mouse = true;
			break;
		case SDL_MOUSEBUTTONUP:
			allInputs.mouse = false;
			break;
		default:
			break;
		}
	}

}

InputStruct Inputs::GetInputs() { //retorna tots els inputs
	return allInputs;
}

void Inputs::TestInputs() { //comprova si funcionen els inputs
	for (std::map<int, bool>::iterator it = allInputs.inputsMap.begin(); it != allInputs.inputsMap.end(); ++it) {
		std::cout << it->first << " " << it->second << std::endl;
	}
	std::cout << "Mouse: " << allInputs.mouse << " Pos: " << allInputs.mousePos.x <<" "<< allInputs.mousePos.y << std::endl;
	std::cout << allInputs.deltaTime << std::endl;
}

void Inputs::UpdateTime() { //actualitza el temps
	allInputs.deltaTime = (clock() - allInputs.lastTime);
	allInputs.lastTime = clock();
	allInputs.deltaTime /= CLOCKS_PER_SEC;
}

Inputs::~Inputs()
{
}

#pragma once
#include <map>
#include "Types.h"
#include <SDL.h>
#include "Constants.h"
#include <time.h>

const int keys[]{ SDLK_w, SDLK_a, SDLK_s, SDLK_d, SDLK_UP, SDLK_DOWN, SDLK_SPACE, SDLK_ESCAPE, SDLK_p };

struct InputStruct {

	std::map<int, bool> inputsMap; //map de les tecles del teclat
	bool mouse, lastMouse, lastEsc; //bools pel mouse i l'ultim estat del mouse
	Vector2 mousePos; //posicio del mouse
	bool end; //si ha d'acabar el programa o no
	float deltaTime;
	clock_t lastTime; //delta time i //ultim estat del rellotge

	InputStruct(std::map<int, bool> inputs, bool _mouse, bool _lastMouse, Vector2 _mousePos, bool _end) :
		inputsMap(inputs), mouse(_mouse), lastMouse(_lastMouse), mousePos(_mousePos), end(_end), deltaTime(clock()), lastTime(clock()) {};

};

class Inputs
{
	InputStruct allInputs; //struct amb tots els inputs 

public:
	Inputs();
	void Update(); //update
	InputStruct GetInputs(); //retorna tots els inputs i derivats
	void UpdateTime(); //actualitza el temps de rellotge
	void TestInputs(); //comprova que tots els inputs funcionen perfectament
	~Inputs();
};


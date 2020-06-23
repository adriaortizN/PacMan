#pragma once
#include "Types.h"
#include "Constants.h"
#include "Renderer.h"
#include <time.h>
#include "Inputs.h"


class PowerUp
{

private:

	Texture texture; //textura
	Rect rect, spriteRect; //rectangle de la posicio i l'sprite
	bool isColliding; //si colisiona o no
	bool isEaten;

public:
	PowerUp(int, int);
	void Update();
	void Eat();
	bool GetIsEaten();
	void Collision();
	Rect GetPosition();		//retorna la posicio 
	void Draw();			//draw
	~PowerUp();
};
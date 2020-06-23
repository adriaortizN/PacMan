#pragma once
#include "Types.h"
#include "Constants.h"
#include "Renderer.h"
#include <time.h>
#include "Inputs.h"

class Wall
{

private:

	Texture texture; //textura
	Rect rect, spriteRect; //rectangle de la posicio i l'sprite
	bool isColliding; //si colisiona o no
	double timeDown; //temps de canvi d'animacio

public:
	Wall(int, int);
	void Collision();
	Rect GetPosition(); //retorna la posicio 
	void Draw();//draw
	~Wall();
};

#pragma once
#include "Types.h"
#include "Constants.h"
#include "Renderer.h"
#include <time.h>
#include "Inputs.h"

class Point
{

private:

	Texture texture; //textura
	Rect rect, spriteRect; //rectangle de la posicio i l'sprite
	int points; //punts que te a dintre, será de 1
	bool isColliding; //si colisiona o no
	bool isEaten;
	int pointsScore;

public:
	Point(int, int);
	void Update(); //update
	void Eat();
	bool GetIsEaten();
	void Collision();
	Rect GetPosition(); //retorna la posicio 
	void Draw();//draw
	~Point();
};

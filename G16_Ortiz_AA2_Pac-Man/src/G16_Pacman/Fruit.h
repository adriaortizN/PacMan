#pragma once
#include "Types.h"
#include "Constants.h"
#include "Renderer.h"
#include <time.h>
#include "Inputs.h"
enum FruitType { CHERRY, STRAWBERRY, ORANGE }; //enum dels tipus de blocs

class Fruit
{

private:

	FruitType type; //tipus de bloc
	Texture texture; //textura
	Rect rect, spriteRect; //rectangle de la posicio i l'sprite
	Vector2 initPos;		//Posicio inicial de la fruita
	int points; //punts que te a dintre
	bool isColliding; //si colisiona o no
	double timeDown; 

public:
	Fruit(FruitType, Vector2);
	void Update(InputStruct); //update
	void Collision();
	int GetPoints(); //retorna els punts que te
	Rect GetPosition(); //retorna la posicio 
	void Draw();//draw
	FruitType GetFruitType();
	~Fruit();
};


#pragma once
#include "Types.h"
#include "Constants.h"
#include "Renderer.h"

class HUDBackground
{

private:

	Texture texture; //textura
	Rect rect, spriteRect; //rectangle de la posicio i l'sprite


public:
	HUDBackground(int, int);
	void Update(); //update
	void Draw();//draw
	Rect GetPosition();
	~HUDBackground();
};


#pragma once
#include "Scene.h"
#include "time.h"

class SplashScreen : public Scene
{
private:
	float timeOut; //temps per sortir de la escena
public:
	SplashScreen();
	void Update(InputStruct); //update
	void Draw(); //draw
	~SplashScreen();
};


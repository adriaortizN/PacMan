#pragma once
#include "Types.h"
#include "SDL.h"
#include "Renderer.h"
#include "Utils.h"
#include "Inputs.h"
#include "TextTexture.h"
#include "Mixer.h"

class Button
{
private:

	//Rect rect; //rectangle del boto
	//std::string textT, defText, hovText; //text, text per defecte i text en hover
	bool pressed; //si esta pulsat o no
	TextTexture *text, *defaultText, *hoverText;

public:
	Button(std::string, Vector2);
	void Update(InputStruct); //update
	bool IsPressed(); //retorna si esta pulsat
	void UpdateText(std::string); //canvia el text
	void Draw();//draw
	~Button();
};


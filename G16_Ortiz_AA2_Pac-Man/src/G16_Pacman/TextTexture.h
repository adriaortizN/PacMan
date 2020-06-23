#pragma once
#include "Types.h"
#include "Renderer.h"
class TextTexture
{
	Text text;
	Rect rect;
	Font font;

public:
	TextTexture(Text, Vector2, Font, ALIGN);
	void UpdateText(std::string);
	std::string GetText();
	Rect GetRect();
	void Draw();
	~TextTexture();
};


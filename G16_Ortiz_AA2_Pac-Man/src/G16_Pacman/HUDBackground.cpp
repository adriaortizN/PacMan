#include "HUDBackground.h"
#include <iostream>

HUDBackground::HUDBackground(int i, int j) : rect{ 0,0,0,0 }, spriteRect{ 0,0,0,0 }, texture{ SPRITESHEET_PATH, "Walls Texture" }
{
	std::cout << i << " " << j << std::endl;
	Renderer * rend = Renderer::Instance();
	rend->LoadTexture(texture.id, texture.path);
	Vector2 size = rend->GetTextureSize(texture.id);
	spriteRect.x = SIZE_TEXTURE * 7;
	spriteRect.y = SIZE_TEXTURE * 6;
	spriteRect.w = size.x / 8;
	spriteRect.h = size.y / 8;

	rect.h = TEXT_SIZE;
	rect.w = TEXT_SIZE;
	rect.x = LIMIT_HUD + (TEXT_SIZE * i);
	rect.y = -TEXT_SIZE + (TEXT_SIZE * j);

}

void HUDBackground::Update() {

}

void HUDBackground::Draw() {

	if (&spriteRect != nullptr) {
		Renderer::Instance()->PushSprite(texture.id, spriteRect, rect); //pinta 
	}

}

Rect HUDBackground::GetPosition() {												//retorna la posicio, si pot
	if (rect.h == NULL || rect.w == NULL || rect.x == NULL || rect.y == NULL) {
		return { 0,0,0,0 };
	}
	else {
		return { rect };
	}

}

HUDBackground::~HUDBackground()
{

}

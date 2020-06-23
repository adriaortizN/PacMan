#include "PowerUp.h"
#include <iostream>

PowerUp::PowerUp(int i, int j) : rect{ 0,0,0,0 }, spriteRect{ 0,0,0,0 }, texture{ SPRITESHEET_PATH, "PowerUp Texture" }
{
	isColliding = false;
	isEaten = false;
	Renderer * rend = Renderer::Instance();
	rend->LoadTexture(texture.id, texture.path);
	Vector2 size = rend->GetTextureSize(texture.id);
	spriteRect.x = SIZE_TEXTURE * 6;
	spriteRect.y = SIZE_TEXTURE * 6;
	spriteRect.w = size.x / 8;
	spriteRect.h = size.y / 8;
	rect.h = TEXT_SIZE;
	rect.w = TEXT_SIZE;
	rect.x = -TEXT_SIZE + (TEXT_SIZE * i);
	rect.y = -TEXT_SIZE + (TEXT_SIZE * j);
}

void PowerUp::Update() {


	if (isColliding) {
		std::cout << "Is colliding" << std::endl;
	}
}

void PowerUp::Draw() {

	if (&spriteRect != nullptr) {
		Renderer::Instance()->PushSprite(texture.id, spriteRect, rect);		//pinta 															
	}

}

Rect PowerUp::GetPosition() {				//retorna la posicio si no es NULL
	if (rect.h == NULL || rect.w == NULL || rect.x == NULL || rect.y == NULL) {
		return { 0,0,0,0 };
	}
	else {
		return { rect };
	}

}

void PowerUp::Eat() {
	isEaten = true;
}

bool PowerUp::GetIsEaten() {
	return isEaten;
}

void PowerUp::Collision() {
	isColliding = true;
}

PowerUp::~PowerUp()
{

}

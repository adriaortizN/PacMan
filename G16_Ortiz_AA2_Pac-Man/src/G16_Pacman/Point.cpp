#include "Point.h"
#include <iostream>

Point::Point(int i, int j) : rect{ 0,0,0,0 }, spriteRect{ 0,0,0,0 }, texture{ SPRITESHEET_PATH, "Point Texture" }
{
	//std::cout << i << " " << j << std::endl;
	isColliding = false;
	isEaten = false;
	pointsScore = 1;
	Renderer * rend = Renderer::Instance();
	rend->LoadTexture(texture.id, texture.path);
	Vector2 size = rend->GetTextureSize(texture.id);
	spriteRect.x = SIZE_TEXTURE * 5;
	spriteRect.y = SIZE_TEXTURE * 6;
	spriteRect.w = size.x / 8;
	spriteRect.h = size.y / 8;
	rect.h = TEXT_SIZE;
	rect.w = TEXT_SIZE;
	rect.x = -TEXT_SIZE + (TEXT_SIZE * i);
	rect.y = -TEXT_SIZE + (TEXT_SIZE * j);

}

void Point::Update() {

	
	if (isColliding) {
		std::cout << "Is colliding" << std::endl;
	}
}

void Point::Draw() {

	if (&spriteRect != nullptr) {
		Renderer::Instance()->PushSprite(texture.id, spriteRect, rect); //pinta 
		//std::cout << "PINTO" << std::endl;
	}

}

Rect Point::GetPosition() { //retorna la posicio si no es NULL
	if (rect.h == NULL || rect.w == NULL || rect.x == NULL || rect.y == NULL) {
		return { 0,0,0,0 };
	}
	else {
		return { rect };
	}

}

void Point::Eat() {
	isEaten = true;
}

bool Point::GetIsEaten() {
	return isEaten;
}

void Point::Collision() {
	isColliding = true;
}

Point::~Point()
{

}

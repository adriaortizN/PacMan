#include "Fruit.h"
#include <iostream>

Fruit::Fruit(FruitType _type, Vector2 _initPos) : type{ _type }, rect{ 0,0,0,0 }, spriteRect{ 0,0,0,0 }, initPos(_initPos), texture{ SPRITESHEET_PATH, "Fruits Texture" }
{
	isColliding = false;
	timeDown = ANIM_TIME;
	Renderer * rend = Renderer::Instance();
	rend->LoadTexture(texture.id, texture.path);
	Vector2 size = rend->GetTextureSize(texture.id);
	
	if (_type == FruitType::CHERRY) {
		spriteRect.x = SIZE_TEXTURE * 0;
		points = 10;
	}else if (_type == FruitType::STRAWBERRY) {
		spriteRect.x = SIZE_TEXTURE * 1;
		points = 15;
	}else if (_type == FruitType::ORANGE) {
		spriteRect.x = SIZE_TEXTURE * 2;
		points = 20;
	}

	
	spriteRect.y = SIZE_TEXTURE * 6;
	spriteRect.w = size.x / 8;
	spriteRect.h = size.y / 8;
	rect.h = TEXT_SIZE;
	rect.w = TEXT_SIZE;
	rect.x = initPos.x * TEXT_SIZE;
	rect.y = initPos.y * TEXT_SIZE;
}

void Fruit::Update(InputStruct inputs) {

	/*
	timeDown -= inputs.deltaTime;
	if (lifes <= 0 && type == FruitType::NORMAL) {
		if (timeDown <= 0) {
			Renderer* rend = Renderer::Instance();
			timeDown = ANIM_TIME;
			if (spriteRect.x >= rend->GetTextureSize(texture.id).x) {
				isColliding = false;
				type = FruitType::NONE;
			}
			else spriteRect.x += spriteRect.w;
		}
	}
	else if (type == FruitType::HEAVY) {
		if (timeDown <= 0) {
			Renderer* rend = Renderer::Instance();
			timeDown = ANIM_TIME;
			if (spriteRect.x >= rend->GetTextureSize(texture.id).x) {
				type = FruitType::NONE;
			}
			else if (spriteRect.x <= spriteRect.w * 2) spriteRect.x = spriteRect.w*(3 - lifes);
			else spriteRect.x += spriteRect.w;
			if (spriteRect.x >= rend->GetTextureSize(texture.id).x) type = FruitType::NONE;
			isColliding = false;
		}
	}
	else if (type == FruitType::FIX && isColliding) {
		if (timeDown <= 0) {
			Renderer* rend = Renderer::Instance();
			timeDown = ANIM_TIME;
			if (spriteRect.x >= rend->GetTextureSize(texture.id).x) {
				isColliding = false;
				spriteRect.x = 0;
			}
			else spriteRect.x += spriteRect.w;


		}
	}
	*/
}

void Fruit::Draw() {
	Renderer::Instance()->PushSprite(texture.id, spriteRect, rect); //pinta si no es del tipus NONE
}

Rect Fruit::GetPosition() { //retorna la posicio si no es del tipus NONE
	return { rect };
}

int Fruit::GetPoints() { //retorna la puntuacio
	return points;
}

void Fruit::Collision() {
	isColliding = true;
}

FruitType Fruit::GetFruitType() {
	return type;
}

Fruit::~Fruit()
{

}

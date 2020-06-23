#include "Wall.h"
#include <iostream>

Wall::Wall(int i, int j) : rect{ 0,0,0,0 }, spriteRect{ 0,0,0,0 }, texture{ SPRITESHEET_PATH, "Walls Texture" }
{
	//std::cout << i << " " << j << std::endl;
	isColliding = false;
	timeDown = ANIM_TIME;
	Renderer * rend = Renderer::Instance();
	rend->LoadTexture(texture.id, texture.path);
	Vector2 size = rend->GetTextureSize(texture.id);
	spriteRect.x = SIZE_TEXTURE * 4;
	spriteRect.y = SIZE_TEXTURE * 6;
	spriteRect.w = size.x / 8;
	spriteRect.h = size.y / 8;

	rect.h = TEXT_SIZE;
	rect.w = TEXT_SIZE;
	rect.x = -TEXT_SIZE + (TEXT_SIZE * i);
	rect.y = -TEXT_SIZE + (TEXT_SIZE * j);
	
}

void Wall::Draw() {

	if(&spriteRect != nullptr){
		Renderer::Instance()->PushSprite(texture.id, spriteRect, rect); //pinta 
	}
	
}

Rect Wall::GetPosition() {																//retorna la posicio, si pot
	if (rect.h == NULL || rect.w == NULL || rect.x == NULL || rect.y == NULL) {
		return { 0,0,0,0 };
	}
	else {
		return { rect };		
	}
	
}

void Wall::Collision() {
	isColliding = true;
}

Wall::~Wall()
{

}

#include "Player.h"
#include <iostream>
#include "Game.h"

#include "../../dep/inc/XML/rapidxml.hpp"
#include "../../dep/inc/XML/rapidxml_utils.hpp"
#include "../../dep/inc/XML/rapidxml_iterators.hpp"
#include "../../dep/inc/XML/rapidxml_print.hpp"
#include <sstream>
#include <time.h>


Player::Player(Direction _direction, int _playerNum, Vector2 _initPos, TextTexture* _scoreText, TextTexture* _cherryText, TextTexture* _strawberryText, TextTexture* _orangeText) : direction(_direction), scoreText(_scoreText), lifesRect{ 0,0,0,0 }, initPos(_initPos), score{ 0 }, lifes{ 3 }, playerNum{ _playerNum }, rect{ 0, 0, 0, 0 },
texture(SPRITESHEET_PATH, "Player Texture"), spriteRect{ 0,0,0,0 }, spriteLivesRect{ 0,0,0,0 }, spriteCherryRect{ 0,0,0,0 }, spriteStrawberryRect{ 0,0,0,0 }, spriteOrangeRect{ 0,0,0,0 },
cherryRect{ 0,0,0,0 }, strawberryRect{ 0,0,0,0 }, orangeRect{ 0,0,0,0 }, cherryText(_cherryText), strawberryText(_strawberryText), orangeText(_orangeText)
{
	isStopped = false;
	Renderer * rend = Renderer::Instance();
	rend->LoadTexture(texture.id, texture.path);
	Vector2 size = rend->GetTextureSize(texture.id); 
	rect.w = lifesRect.w= spriteRect.w = size.x;
	rect.h = lifesRect.h= spriteRect.h = size.y;

	spriteRect.x = SIZE_TEXTURE * 0;
	spriteRect.y = SIZE_TEXTURE * 0;

	spriteLivesRect.w = spriteRect.w = size.x / 8;
	spriteLivesRect.h = spriteRect.h = size.y / 8;

	spriteLivesRect.x = SIZE_TEXTURE * 6;		//Donat que els pacman del HUD no tenen d' estar animats ni tenir el mateix sprite que el jugador, fico el sprite unic i bo
	spriteLivesRect.y = SIZE_TEXTURE * 0;

	rect.h = TEXT_SIZE;
	lifesRect.h = rect.h * 2;
	rect.w = TEXT_SIZE;
	lifesRect.w = rect.w * 2;

	lifesRect.x = LIFESRECTX;
	lifesRect.y = LIFESRECTY;
	

	
	rect.x = initPos.x * TEXT_SIZE;
	rect.y = initPos.y * TEXT_SIZE;
		
	
	

	//Cooldown de l' animacio a cero 
	cooldownAnimation = 0.0;

	//Preparar tamaños y posicion de cherry
	spriteCherryRect.w = size.x / 8;
	spriteCherryRect.h = size.y / 8;
	spriteCherryRect.x = SIZE_TEXTURE * 0;			
	spriteCherryRect.y = SIZE_TEXTURE * 6;

	cherryRect.h = rect.h * 2;
	cherryRect.w = rect.w * 2;
	cherryRect.x = LIFESRECTX;
	cherryRect.y = LIFESRECTY - 400;

	cherryCount = 0;

	//Preparar tamaños y posicion de strawberry
	spriteStrawberryRect.w = size.x / 8;
	spriteStrawberryRect.h = size.y / 8;
	spriteStrawberryRect.x = SIZE_TEXTURE * 1;			
	spriteStrawberryRect.y = SIZE_TEXTURE * 6;

	strawberryRect.h = rect.h * 2;
	strawberryRect.w = rect.w * 2;
	strawberryRect.x = LIFESRECTX;
	strawberryRect.y = LIFESRECTY - 325;

	strawberryCount = 0;

	//Preparar tamaños y posicion de orange
	spriteOrangeRect.w = size.x / 8;
	spriteOrangeRect.h = size.y / 8;
	spriteOrangeRect.x = SIZE_TEXTURE * 2;			
	spriteOrangeRect.y = SIZE_TEXTURE * 6;

	orangeRect.h = rect.h * 2;
	orangeRect.w = rect.w * 2;
	orangeRect.x = LIFESRECTX;
	orangeRect.y = LIFESRECTY - 250;

	orangeCount = 0;
	

	anim = 0.0;
}

void Player::Update(InputStruct inputs) { //si esta dintre dels limits de joc es pot moure
	
	if (inputs.inputsMap[keys[ACTIONS::W_KEY]]) {
		direction = W;	
		spriteRect.x = SIZE_TEXTURE * 0;				//Cambio l' sprite sino hi ha una mica de delay i queden les animacions malament
		anim = 0.0;										//Resetejo el temps d' animacio pk les animacions siguin mes fluides
	}
	else if (inputs.inputsMap[keys[ACTIONS::A_KEY]]) {
		direction = A;
		spriteRect.x = SIZE_TEXTURE * 6;				
		anim = 0.0;
	}
	else if (inputs.inputsMap[keys[ACTIONS::S_KEY]]) {
		direction = S;
		spriteRect.x = SIZE_TEXTURE * 2;				
		anim = 0.0;
	}
	else if (inputs.inputsMap[keys[ACTIONS::D_KEY]]) {
		direction = D;
		spriteRect.x = SIZE_TEXTURE * 4;				
		anim = 0.0;	
	}

}

void Player::Draw() 
{
	Renderer::Instance()->PushSprite(texture.id, spriteRect, rect); //pinta l'sprite
	Rect tmpRect{ lifesRect };
	for (int i = 0; i < lifes; i++) { //pinta les vides de manera consecutiva
		tmpRect.x = lifesRect.x +(70 * i);

		Renderer::Instance()->PushSprite(texture.id, spriteLivesRect, tmpRect);
	}
	Renderer::Instance()->PushSprite(texture.id, spriteCherryRect, cherryRect);
	Renderer::Instance()->PushSprite(texture.id, spriteStrawberryRect, strawberryRect);
	Renderer::Instance()->PushSprite(texture.id, spriteOrangeRect, orangeRect);

	scoreText->Draw(); //pinta la seva puntuacio
	cherryText->Draw();
	strawberryText->Draw();
	orangeText->Draw();
}

void Player::SetPlayerPos(Rect _rect) {

	
	rect.x = _rect.x;
	rect.y = _rect.y;
	
	switch (direction) {
	case W:
		if(!isStopped){

			anim += ANIM_TIME;
				if(anim > MAX_ANIM_TIME){
				spriteRect.x += SIZE_TEXTURE;
				if (spriteRect.x >= SIZE_TEXTURE * 2) {
					spriteRect.x = SIZE_TEXTURE * 0;
				}
				
				anim = 0.0;
			}
			if (Player::GetPlayerPos().y < TEXT_SIZE) {			//Maps limit
				Player::rect.y = TEXT_SIZE;
			}
			if (Player::GetPlayerPos().y > YLIMIT) {			//Maps limit
				Player::rect.y = YLIMIT - TEXT_SIZE;
			}
			
		}
		break;
	case A:
		if (!isStopped) {

			anim += ANIM_TIME;
			if (anim > MAX_ANIM_TIME){
				spriteRect.x += SIZE_TEXTURE;
				if (spriteRect.x >= SIZE_TEXTURE * 8) {
					spriteRect.x = SIZE_TEXTURE * 6;
				}
				anim = 0.0;
			}
			if (Player::GetPlayerPos().x < TEXT_SIZE) {			//Maps limit
				std::cout << rect.y << std::endl;
				if (rect.y >= 380 && rect.y <= 401) {			//Les walls del costat més a l'esquerra no funcionen correctament. Per tant, haig de fer aixó perque pugui pasar per el cami que transporta a l' altre banda
					if (Player::GetPlayerPos().x < 0) {			//Maps limit	
						Player::rect.x = XLIMIT - TEXT_SIZE;
					}
				}
				else {
					direction = STOP;							//Si arriba a aquella posicio, vol dir que hi ha una wall
				}
			}
			if (Player::GetPlayerPos().y > YLIMIT) {			//Maps limit
				Player::rect.y = YLIMIT - TEXT_SIZE;
			}
		}
		
		break;
	case S:
		if (!isStopped) {
			anim += ANIM_TIME;
			if(anim > MAX_ANIM_TIME){
				spriteRect.x += SIZE_TEXTURE;
				if (spriteRect.x >= SIZE_TEXTURE * 4) {
					spriteRect.x = SIZE_TEXTURE * 2;
				}
				anim = 0.0;
			}
			if (Player::GetPlayerPos().y > YLIMIT) {			//Maps limit
				Player::rect.y = YLIMIT - TEXT_SIZE;
			}
		}
		break;
	case D:
		if (!isStopped) {
			anim += ANIM_TIME;
			if(anim > MAX_ANIM_TIME){
				spriteRect.x += SIZE_TEXTURE;
				if (spriteRect.x >= SIZE_TEXTURE * 6) {
					spriteRect.x = SIZE_TEXTURE * 4;
				}
				anim = 0.0;
			}
			if (Player::GetPlayerPos().x > XLIMIT - TEXT_SIZE) {			//Maps limit
				Player::rect.x = 0;
			}

			if (Player::GetPlayerPos().y > YLIMIT) {			//Maps limit
				Player::rect.y = YLIMIT - TEXT_SIZE;
			}
		}
		break;
	case STOP:

		break;
	}
}

void Player::TransportPlayer(Rect _rect) {
	rect.x = _rect.x;
	rect.y = _rect.y;
}

Rect Player::GetPlayerPos() { //retorna la posicio del jugador
	return{ rect };
}


void Player::ReduceLife() { //redueix la vida
	lifes--;
}

int Player::GetScore() {//retorna l'score
	return score;
}

int Player::GetLifes() { //retorna les vides
	return lifes;
}

int Player::GetCherry() {
	return cherryCount;
}

int Player::GetStrawberry() {
	return strawberryCount;
}

int Player::GetOrange() {
	return orangeCount;
}

void Player::addScore(int incomingScore) { //afegeix puntuacio
	score += incomingScore;
	//std::cout << score << std::endl;
	if (score < 0) score = 0;
	scoreText->UpdateText(std::to_string(score));
}

void Player::addCherry() {
	cherryCount += 1;

	cherryText->UpdateText("X " + std::to_string(cherryCount));
}

void Player::addStrawberry() {
	strawberryCount += 1;

	strawberryText->UpdateText("X " + std::to_string(strawberryCount));
}

void Player::addOrange() {
	orangeCount += 1;

	orangeText->UpdateText("X " + std::to_string(orangeCount));
}



Player::~Player()
{
}

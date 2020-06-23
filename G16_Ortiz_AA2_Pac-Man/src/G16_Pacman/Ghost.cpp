#include "Ghost.h"
#include <iostream>
#include <sstream>
#include <time.h>


Ghost::Ghost(GhostDirection _direction, int _ghostNum, Vector2 _initPos, GhostType _ghostType) : direction(_direction), ghostNumber{ _ghostNum }, initPos (_initPos),
ghostType(_ghostType), rect{ 0, 0, 0, 0 }, texture(SPRITESHEET_PATH, "Ghost Texture"), spriteRect{ 0,0,0,0 } {

	ghostIsVulnerable = false;
	Renderer * rend = Renderer::Instance();
	rend->LoadTexture(texture.id, texture.path);
	Vector2 size = rend->GetTextureSize(texture.id);

	rect.w = spriteRect.w = size.x;
	rect.h = spriteRect.h = size.y;
	
	spriteRect.x = SIZE_TEXTURE * 0;

	direction = _direction;

	switch (ghostType) {
	case BLINKY:
			spriteRect.y = SIZE_TEXTURE * 1;
		break;
	case INKY:
			spriteRect.y = SIZE_TEXTURE * 2;
		break;
	case CLYDE:
			spriteRect.y = SIZE_TEXTURE * 3;
		break;
	}

	spriteRect.w = size.x / 8;
	spriteRect.h = size.y / 8;

	rect.h = TEXT_SIZE;				//Tamany dels fantasmes 
	rect.w = TEXT_SIZE;

	rect.x = initPos.x * TEXT_SIZE;
	rect.y = initPos.y * TEXT_SIZE;

	anim = 0.0;
}


void Ghost::Draw() {

	if (&spriteRect != nullptr) {
		Renderer::Instance()->PushSprite(texture.id, spriteRect, rect); //pinta 
	}

}

void Ghost::Update(GhostDirection _direction) {

	anim += ANIM_TIME;

	//std::cout << anim << std::endl;

	if (ghostIsVulnerable) {						//Si el fantasma es vulnerable, es té de dibuixar el sprite vulnerable

		spriteRect.y = SIZE_TEXTURE * 4;
		if (anim > MAX_ANIM_TIME) {
			spriteRect.x += SIZE_TEXTURE;
			if (spriteRect.x >= SIZE_TEXTURE * 3) {
				spriteRect.x = SIZE_TEXTURE * 0;
			}

			anim = 0.0;
		}
	}
	else {											//Si no ho es, es coloca la seva Y al spritesheet segons el seu tipus
		switch (ghostType) {
		case BLINKY:
			spriteRect.y = SIZE_TEXTURE * 1;
			break;
		case INKY:
			spriteRect.y = SIZE_TEXTURE * 2;
			break;
		case CLYDE:
			spriteRect.y = SIZE_TEXTURE * 3;
			break;
		}

		switch (_direction) {						//I la seva direccio segons a on estigui dirigintse
		case WGHOST:

			if (anim > MAX_ANIM_TIME) {
				spriteRect.x += SIZE_TEXTURE;
				if (spriteRect.x >= SIZE_TEXTURE * 2) {
					spriteRect.x = SIZE_TEXTURE * 0;;
				}

				anim = 0.0;
			}

			break;
		case AGHOST:

			if (anim > MAX_ANIM_TIME) {
				spriteRect.x += SIZE_TEXTURE;
				if (spriteRect.x >= SIZE_TEXTURE * 8) {
					spriteRect.x = SIZE_TEXTURE * 6;;
				}

				anim = 0.0;
			}
			break;
		case SGHOST:

			if (anim > MAX_ANIM_TIME) {
				spriteRect.x += SIZE_TEXTURE;
				if (spriteRect.x >= SIZE_TEXTURE * 4) {
					spriteRect.x = SIZE_TEXTURE * 2;;
				}

				anim = 0.0;
			}
			break;
		case DGHOST:
			if (anim > MAX_ANIM_TIME) {
				spriteRect.x += SIZE_TEXTURE;
				if (spriteRect.x >= SIZE_TEXTURE * 6) {
					spriteRect.x = SIZE_TEXTURE * 4;;
				}

				anim = 0.0;
			}
			break;
		case STOPGHOST:

			break;
		}

	}

	
		
	
	//std::cout << "UPDATE" << std::endl;
}

Rect Ghost::GetGhostPos() {
	return{ rect };
}

GhostType Ghost::GetGhostType() {
	return ghostType;
}

void Ghost::SetGhostPos(Rect _rect) {
	rect.x = _rect.x;
	rect.y = _rect.y;

	if (GetGhostPos().y < TEXT_SIZE) {							//Maps limit
		rect.y = TEXT_SIZE;
	}

	if (GetGhostPos().x < 0) {									//Maps limit	
		rect.x = XLIMIT - TEXT_SIZE;
	}

	if (GetGhostPos().y > YLIMIT) {								//Maps limit
		rect.y = YLIMIT - TEXT_SIZE;
	}

	if (GetGhostPos().x > XLIMIT - TEXT_SIZE) {					//Maps limit
		rect.x = 0;
	}

}

void Ghost::SetDirectionGhost(GhostDirection _GhostDirection) {

	direction = _GhostDirection;
}

GhostDirection Ghost::GetDirectionGhost() {
		return direction;
}

void Ghost::SwitchGhostIsVulnerable(bool _IsVulnerable) {
	if (_IsVulnerable) {
		ghostIsVulnerable = true;
	}
	else {
		ghostIsVulnerable = false;
	}
	/*
	if (ghostIsVulnerable) {
		ghostIsVulnerable = false;
	}
	else {
		ghostIsVulnerable = true;
	}
	*/
	//return { ghostIsVulnerable };
}

bool Ghost::GetGhostIsVulnerable() {
	return ghostIsVulnerable;
}

Ghost::~Ghost() {

}




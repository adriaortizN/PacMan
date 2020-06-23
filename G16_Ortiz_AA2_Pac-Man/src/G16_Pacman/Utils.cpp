#include "Utils.h"


bool isColliding(Vector2 vector, Rect rect) { //retorna true si el vector colisiona amb el rectangle

	return (vector.x > rect.x && vector.x < rect.x + rect.w && vector.y > rect.y && vector.y < rect.y + rect.h);

}

bool isColliding(Rect rect1, Rect rect2) {

	return !((rect1.x < rect2.x < (rect1.x + rect1.w) || rect1.x < (rect2.x + rect2.w) < (rect1.x + rect1.w)) &&
		(rect1.y < rect2.y < (rect1.y + rect1.h) || rect1.y < (rect2.y + rect2.h) < (rect1.y + rect1.h)));

}

bool CenteredColliding(Vector2 vector, Rect rect) { //retorna si el vector colisiona amb un rectangle amb el pivot al centre
	return(vector.x > rect.x && vector.x < rect.x + rect.h && vector.y > rect.y && vector.y < rect.y + rect.w);
}

int CenteredCollidingDir(Vector2 vector, Vector2 lastvector, Rect rect)
{
	if (vector.x > rect.x && vector.x < rect.x + rect.h && vector.y > rect.y && vector.y < rect.y + rect.w) {
		if (lastvector.x < rect.x && lastvector.y > rect.y && lastvector.y < rect.y + rect.w) return 0; //LEFT
		else if (lastvector.y < rect.y && lastvector.x > rect.x && lastvector.x < rect.x + rect.h) return 1; //TOP
		else if (lastvector.y > rect.y + rect.w  && lastvector.x > rect.x && lastvector.x < rect.x + rect.h) return 2; //BOTTOM
		else return 3; //RIGHT
	}
	return -1;
}


bool CenteredColliding(Rect rect1, Rect rect2) { //retorna si el vector colisiona amb un rectangle amb el pivot al centre
	return(rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.h > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.y + rect1.w > rect2.y);
}


bool ExitLimitsV(Vector2 vector, Rect rect) { //retorna si el vector surt per l'eix vertical 
	return (vector.y < rect.y || vector.y > rect.y + rect.h);
}

int ExitLimitsH(Vector2 vector, Rect rect) { //retorna si el vector surt per l'eix horitzontal i per on
	if (vector.x < rect.x) return 0;
	else if (vector.x > rect.x + rect.w) return 1;
	else return -1;
}

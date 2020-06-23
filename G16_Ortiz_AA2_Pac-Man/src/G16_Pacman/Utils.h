#pragma once
#include "Types.h"

bool isColliding(Vector2, Rect); //retorna true el vector esta dintre del rectangle

bool isColliding(Rect, Rect); //retorna true si dos rectangulos estan colisionando 

bool CenteredColliding(Vector2, Rect); // retorna true si colisionen amb rectangles centrats

int CenteredCollidingDir(Vector2, Vector2, Rect); //retorna true per on colisiona -1 si no colisiona

bool CenteredColliding(Rect, Rect); // retorna true si colisionen amb rectangles centrats

int ExitLimitsH(Vector2, Rect); //retorna si ha sortit del limit per l'eix horitzontal i per quina banda

bool ExitLimitsV(Vector2, Rect); //retorna si ha sortit del limit per l'eix vertical
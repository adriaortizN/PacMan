#pragma once
#include "Types.h"
#include "Renderer.h"
#include "Inputs.h"
#include "Utils.h"
#include "TextTexture.h"
#include "Wall.h"
enum Direction { W, A, S, D, STOP };

class Player
{
private:

	int playerNum;				//numero de jugador
	Texture texture;			//textura
	Rect rect;					//rectangle
	Rect spriteRect;			//rectangle de l'sprite
	Rect spriteLivesRect;		//rectangle de l'sprite de vides
	Rect spriteCherryRect;		//rectangle de l'sprite de la cirera
	Rect spriteStrawberryRect;	//rectangle de l' sprite de la maduixa
	Rect spriteOrangeRect;		//rectangle de l' sprite de la taronja
	Rect lifesRect;				//posicio de les vides		
	Rect cherryRect;			//posició de les cireres
	Rect strawberryRect;		//posicio de la maduixa
	Rect orangeRect;			//posicio de la taronja

	int lifes; //quantitat de vides			
	int score; //puntuacio					
	float speed; //velocitat
	Vector2 initPos;

	int cherryCount;
	int strawberryCount;
	int orangeCount;

	float anim;
	
	
	TextTexture* scoreText; //punter a la puntuació

	TextTexture* cherryText; //punter al text de cherry
	TextTexture* strawberryText; //punter al text de cherry
	TextTexture* orangeText; //punter al text de cherry

	float cooldownAnimation;


public:
	

	Player(Direction, int, Vector2, TextTexture*, TextTexture*, TextTexture*, TextTexture*);
	void Update(InputStruct); //update
	void SetPlayerPos(Rect);	//Setea la dirección hacia donde se dirigirá el jugador
	Rect GetPlayerPos(); //retorna la posicio del jugador
	void TransportPlayer(Rect);	//Teletransporta al jugador a una posició en especific. Util per fer l' spawn;
	void ReduceLife(); //resta una vida
	void addScore(int); //afegeix puntuacio a l'score que te
	int GetLifes(); //retorna les vides del jugador
	int GetScore(); //retorna la puntuacio del jugador
	int GetCherry();
	void addCherry();
	int GetStrawberry();
	void addStrawberry();
	int GetOrange();
	void addOrange();

	void Draw(); //draw

	Direction direction;
	bool isStopped;

	~Player();
};


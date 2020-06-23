#pragma once
#include "Scene.h"
#include "Player.h"
#include "Inputs.h"
#include "Button.h"
#include "Wall.h"
#include "PowerUp.h"
#include "Point.h"
#include "Utils.h"
#include "HUDBackground.h"
#include "Fruit.h"
#include "Ghost.h"

enum PlayGameSceneState { START_GAME, RUNNING, PAUSE, GAME_OVER };
enum Axis { GOUP, GODOWN, GORIGHT, GOLEFT };
enum GridType { WALL, NOTHING};

class Game :
	public Scene
{
private:
	Wall* gridWalls[21][21];		//grid de blocs
	Point* gridPoints[21][21];		//grid del points
	PowerUp* gridPowerUps[21][21];	//grid de los power Ups
	HUDBackground* gridHUD[8][21];	//grid del fons esquerra del hud
	GridType gridType[21][21];		//Grid para las colisiones

	Ghost *ghost[5];				//Array dels fantasmes, per guardarlos
	float ghostTimer[2];
	GhostDirection* ghostDirection;
	GhostDirection ghostPosiblesDirections[3];

	Rect limits; //limits del joc
	PlayGameSceneState gameSceneState; //estat de la escena del joc
	Axis axis;
	Player *player; //player 

	Vector2 playerPos;
	Vector2 playerLeftUp;		//Per les colisions, punts del cuadrat del sprite
	Vector2 playerRightUp;
	Vector2 playerLeftDown;
	Vector2 playerRightDown;	
	Vector2 centerPos;			//Posició central, per corregir al jugador al girar cap una paret
	float delaySpawn;			//Delay per fer spawn del pacman (al morir)

	Vector2 BlinkyPos;			//Posiciones de los fantasmas, sobretodo utilizado para guardar su posición inicial y setearla cuando vuelvan a hacer spawn
	Vector2 InkyPos;			
	Vector2 ClydePos;	

	Vector2 ghostLeftUp;		//Per les colisions, punts del cuadrat del sprite
	Vector2 ghostRightUp;
	Vector2 ghostLeftDown;
	Vector2 ghostRightDown;

	Vector2 PowerUpPos;


	
	Button* soundButton; //boto de so
	
	std::string winnerName;		//Nom del jugador pel ranking. No implementat
	int winnerScore;			//Puntuacio del jugador pel ranking. Es guarda pero no s' aplica a cap lloc. 
	TextTexture * title, *sideTitle, *pauseTitle; //textos
	bool isPointEated;	//Si el point ha ha sigut menjat per el jugador
	int pointsToWin;	//Punts per completar el mapa
	int pointsInMap = 0;

	Rect * playerVariables;		//Rect per realitzar les colisions 
	Rect * tempPlayerVariables;
	Rect * wallVariables;
	Rect * pointsVariables;
	Rect * powerUpsVariables;
	Rect * fruitVariables;
	Rect * ghostVariables;

	bool ghostIsVulnerable;			//Si el fantasma te el power down. No utilitzat, encara 
	GhostType ghostType;			//Per saber de quin tipus es el fantasma a la hora de fer el moviment 
	GhostDirection ghostDirections;
	float powerUpsTime;				//Temps restant del power UP
	bool isRespawningGhost[2];
	
	float cooldownFruit;
	Fruit* fruit;
	FruitType fruitType;
	FruitType previousFruitType;	//Per evitar que spawneji el mateix tipus de fruita que l' anterior

	int incomingPoints;		//Punts de la fruit que rebra el player

	int XCollisions;		//"Cela" del player en l' eix de les X
	int YCollisions;		//"Cela" del player en l' eix de les Y
	int random;				//Numero a utilizar per randoms, com el spawn de fruites 


public:
	Game();
	void Update(InputStruct);	//update
	void TestState();			//testeja l'estat en el que es troba
	void LoadGrid();			//carrega el grid d'un xml
	void LoadHUD();				//guarda el HUD en un vector 
	void SetPlayerPos();
	void SetGhostPos();
	void GhostUpdate();

	void CheckPointsCollision();
	void CheckFruitCollision();
	void CheckGhostsCollision();
	void CheckPowerUpsCollision();
	void RespawnPlayer();		//Funcio per quan el player hagi sigut tocat per un fantasma
	void RespawnGhost(int);		//Funcio per quan el ghost hagi sigut tocat per el jugador quan estava vulnerable 

	void GhostPowerUpsFunction();		//Comprovació dels powerUps
	void MoveBlinkyGhost(GhostDirection [], int, int);
	void ReinitVariablesGhost();

	bool CheckGridCollision(Axis, Rect);

	float TimerFruit(InputStruct, float);
	void SpawnFruit(float);

	bool CheckLimits(Vector2); //comprova si el vector esta dintre dels limits
	std::pair<std::string, int> GetWinner();
	void Draw(); //draw
	~Game();
};


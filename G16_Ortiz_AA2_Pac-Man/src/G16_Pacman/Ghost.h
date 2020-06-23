#include "Types.h"
#include "Renderer.h"
#include "Inputs.h"
#include "Utils.h"

enum GhostType { BLINKY, INKY, CLYDE };
enum GhostDirection { WGHOST, AGHOST, SGHOST, DGHOST, STOPGHOST};

class Ghost
{
private:

	int ghostNumber;			//numero del fantasma 
	Vector2 initPos;			//Posicio inicial del fantasma
	Texture texture;			//textura
	Rect rect;					//rectangle
	Rect spriteRect;			//rectangle de l'sprite
	GhostType ghostType;		//Tipus de fantasma 


	
	float speed;				//velocitat

	bool ghostIsVulnerable;		//Si el fantasma te el powerDown actiu. 

	float anim;					//Temps per l' animacio

	GhostDirection direction;	//Direcio del fantasma per realitzar les animacions


public:


	Ghost(GhostDirection, int, Vector2, GhostType);	//Constructor
	void Update(GhostDirection);					//update
	void SetGhostPos(Rect);							//Setea la dirección hacia donde se dirigirá el jugador
	Rect GetGhostPos();								//retorna la posicio del jugador
	GhostType GetGhostType();						//Retorna el tipos de ghost que es
	bool GetGhostIsVulnerable();					//Retorna si el fantasma esta vulnerable
	void SwitchGhostIsVulnerable(bool);						//Retorna si el fantasma esta vulnerable

	void SetDirectionGhost(GhostDirection);			//Mou el fantasma
	GhostDirection GetDirectionGhost();				//Obte la direccio del fantasma


	void Draw();
	~Ghost();
};
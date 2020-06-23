#pragma once
#include <SDL.h>
#include <string>

#include "Types.h"
//Screen dimensions
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700

//time
const int FPS = 60;
const int MAX_TIMEDOWN = 60;
const int DELAY_TIME = 1000.f / FPS;
//window name
static const char * WINDOW_NAME = "Pacman";

//Tamany de una textura, ideal per no anant ficant 35. 
const int TEXT_SIZE = 35;

//Actors Speed
const int PACMAN_SPEED = 3;

//Player Limit in X axis
const int XLIMIT = 675;

//Player Limit in Y axis
const int YLIMIT = 700 - (TEXT_SIZE * 2) + 5;

//Posición en el mapa de las vidas
const int LIFESRECTX = 695;
const int LIFESRECTY = 600;

//Points a agafar per acabar partida
const int MAX_POINTS = 194;


//paths
//img paths
const std::string SPLASHSCREEN_PATH{ "../../res/img/bg.jpg" };
const std::string TRANSPARENCY_PATH{ "../../res/img/bTrans.png" };
//animated paths
const int SIZE_TEXTURE = 128;													//Tamany de qualsevol imatge del spritesheet 
const std::string SPRITESHEET_PATH{ "../../res/img/PacManSpritesheet.png" };
const std::string BACKGROUND_PATH{ "../../res/img/Background.png" };


//file paths
const std::string XML_PATH{ "../../res/files/config.xml" };						//Direccio del xml
const std::string BIN_PATH{ "../../res/files/ranking.bin" };					//Direccio per guardar el ranking

//font paths
const std::string FONT_PATH{ "../../res/ttf/PAC-FONT.ttf" };
const std::string GAMEPLAY_PATH{ "../../res/ttf/Gameplay.ttf" }; 


//audio
const std::string AUDIO_PATH{ "../../res/au/music.mp3" };
const int AUDIO_FREQUENCY{ 50000 };
const std::string AUDIOPLAY_PATH{ "../../res/au/music.mp3" }; 
//gamestate
enum class GameState {SPLASHSCREEN, MENU, GAME, RANKING, EXIT};		//Estas posibles del joc 

//splashScreen
	//time
	const int SPLASHSCREEN_TIME = 3;								//Temps que es mostra la splash screen 

//colors
	//white color
	const Color COLOR_WHITE{ 255,255,255,255 };

	//green color
	const Color COLOR_GREEN{ 0,255,0,255 };

	//red color
	const Color COLOR_RED{ 255,0,0,255 };

	//black color
	const Color COLOR_BLACK{ 0,0,0,255 };

//keys
const enum ACTIONS { W_KEY, A_KEY, S_KEY, D_KEY, UP_KEY, DOWN_KEY, SPACEBAR, ESCAPE, P_KEY };	//Inputs que pot realizar el jugador
//const int keys[]{ SDLK_w, SDLK_s, SDLK_UP, SDLK_DOWN, SDLK_SPACE, SDLK_ESCAPE, SDLK_p };

//limits
const Rect LIMITS_GAME{0, 0, 900, 700};		//Limits del joc
const int LIMIT_HUD = 700;					//Limit del HUD

//anim
const float ANIM_TIME{0.05f};				//Temps d' animacio 
const float MAX_ANIM_TIME{ 0.15f };			//Maxima durada de un sprite sense canviar l' animació 

//time
const float FRAME_TIME{ 0.016f };

//fonts
const Font TITLE_FONT{ FONT_PATH , "Title Font", 95 };
const Font TITLELITTLE_FONT{ FONT_PATH , "TitleLittle Font", 40 };
const Font DEFAULT_FONT{ GAMEPLAY_PATH , "Default Font", 40 };

//align text
const enum ALIGN {LEFT, CENTER, RIGHT};

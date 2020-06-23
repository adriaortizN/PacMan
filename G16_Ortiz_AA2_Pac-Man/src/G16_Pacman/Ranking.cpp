#include "Ranking.h"


//Creación del tamaño de los botones 
Ranking::Ranking(std::pair<std::string, int> _score) : exitButton(new Button("Exit", Vector2{ 390, 500 })),
title(new TextTexture{ { "Title Text", "Ranking", COLOR_GREEN },{ SCREEN_WIDTH / 2, 50 }, TITLELITTLE_FONT, ALIGN::CENTER })	//Titulo del juego 
{
	stateScene = GameState::RANKING;
	//mixer = new Mixer{ AUDIO_PATH, AUDIO_FREQUENCY };
	//Renderer::Instance()->LoadTexture("BG_GENERAL", BACKGROUND_PATH); //posa la textura del joc en memoria

	stateScene = GameState::RANKING;
	exitButton = new Button("Return to menu", Vector2{ 250, 600 });




	std::ifstream fread(BIN_PATH, std::ios::in | std::ios::binary);
	while (!fread.is_open()) {	//crea un fitxer si no existeix
		CreateRankingFile(); //crida la funcio de crear el ranking
		fread.open(BIN_PATH, std::ios::in | std::ios::binary);
	}
	int size;
	fread.read(reinterpret_cast<char*>(&size), sizeof(size)); //llegeix el primer valor del ranking que es la size
	for (int i = 0; i < size; i++) {
		size_t len; //agafa la size de l'string
		std::string name; //agafa l'string
		int score; //agafa la puntuacio
		fread.read(reinterpret_cast<char*>(&len), sizeof(size_t));
		char* temp = new char[len + 1];
		fread.read(temp, len);
		temp[len] = '\0';
		name = temp;
		delete[] temp;
		fread.read(reinterpret_cast<char*>(&score), sizeof(score));
		rank.push_back({ name, score }); //el fa push al vector de ranks
	}
	fread.close();
	bool exists = false;
	for (int i = 0; i < rank.size(); i++) //itera en el ranking per saber si la nova puntuacio s'ha de reescriure o no
	{
		if (rank[i].first == _score.first)
		{
			exists = true;
			if (rank[i].second < _score.second)
				rank[i] = _score;
		}
	}
	if (!exists) rank.push_back(_score); //si no existeix fa el push
	OrderRanking(); //ordena el ranking amb la nova puntuacio
	if (!exists) rank.pop_back(); //fa pop de la ultim valor
	SaveRanking(); //guarda el ranking
	Renderer::Instance()->LoadTexture("BG_GENERAL", BACKGROUND_PATH); //posa la textura del joc en memoria
	int c = 0;
	Color rankColor = COLOR_WHITE;
	for (int i = 0; i < rank.size(); i++) { //crea una textura per a cada ranking
		if (rank[i].first == _score.first) rankColor = COLOR_GREEN;
		else rankColor = COLOR_WHITE;
		Text tmptext{ "Score " + std::to_string(c), "" + rank[i].first + ": " + std::to_string(rank[i].second), rankColor };//crea text per la escena de joc
		scores.push_back(new TextTexture{ tmptext,{ 700, 175 + 75 * c }, DEFAULT_FONT , ALIGN::RIGHT });
		c++;
	}
}


void Ranking::Update(InputStruct inputs)
{
	exitButton->Update(inputs);
	if (exitButton->IsPressed()) stateScene = GameState::MENU; //actualitza el boto i si es pressiona passa a l'estat MENU
}

void Ranking::Draw() { //draw

	Renderer* rend = Renderer::Instance();
	rend->PushImage("BG_GENERAL", Rect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT }); //pinta el fons

	exitButton->Draw(); //pinta el boto
	title->Draw();		//pinta el titol de l' escena

	for (int i = 0; i < scores.size(); i++) {												//pinta el ranking
		scores[i]->Draw();
	}
}

void Ranking::CreateRankingFile() {															//crea un arxiu de ranking amb 5 valors a cero

	std::ofstream fwrite(BIN_PATH, std::ios::out | std::ios::binary);
	int n = 5;
	fwrite.write(reinterpret_cast<char *>(&n), sizeof(n));
	for (int i = 0; i < n; i++) {
		std::string str = "Score" + std::to_string(i);
		size_t len = str.size();
		fwrite.write(reinterpret_cast<char *>(&len), sizeof(len));
		fwrite.write(str.c_str(), str.size());
		int n2 = 0;
		fwrite.write(reinterpret_cast<char *>(&n2), sizeof(n2));
	}
	fwrite.close();

}

void Ranking::SaveRanking() {																//guarda el ranking
	std::ofstream fwrite(BIN_PATH, std::ios::out | std::ios::binary);
	int n = 5;
	fwrite.write(reinterpret_cast<char *>(&n), sizeof(n));
	for (int i = 0; i < n; i++) {
		std::string str = rank[i].first;
		size_t len = str.size();
		fwrite.write(reinterpret_cast<char *>(&len), sizeof(len));
		fwrite.write(str.c_str(), str.size());
		int n2 = rank[i].second;
		fwrite.write(reinterpret_cast<char *>(&n2), sizeof(n2));
	}
	fwrite.close();
}

void Ranking::OrderRanking() { //endresa el ranking
	std::pair < std::string, int> tmp;
	for (int i = 1; i < rank.size(); i++) {
		for (int j = 0; j < (rank.size() - i); j++) {
			if (rank[j].second < rank[j + 1].second) {
				tmp = rank[j];
				rank[j] = rank[j + 1];
				rank[j + 1] = tmp;
			}
		}
	}
}

Ranking::~Ranking()
{
}
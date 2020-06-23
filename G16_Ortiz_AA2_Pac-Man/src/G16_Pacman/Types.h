#pragma once
#include <string>
#include <map>

struct Vector2 { //vector de 2 dimensions

	int x, y;

	Vector2(int _x, int _y) : x(_x), y(_y) {}

};

struct Font { //guarda el path, la id i el tamany de la font

	std::string path, id;

	int size;

	Font(std::string _path, std::string _id, int _size) : path(_path), id(_id), size(_size) {}

};

struct Color { //guarda un color

	unsigned char r, g, b, a;

	Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : r(_r), g(_g), b(_b), a(_a) {}


};

struct Text {//guarda text, la id i el color

	std::string text, id;

	Color color;

	Text(std::string _id, std::string _text, Color _color) : id(_id), text(_text), color(_color) {}

};

struct Rect {//rectangle

	int x, y;
	int w, h;

	Rect(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {};

};

struct CenteredRect { //rectangle que te el centre al mig
	int x, y;
	int w, h;

	CenteredRect(int _x, int _y, int _w, int _h) : x(_x + (_w / 2)), y(_y + (_h / 2)), w(_w / 2), h(_h / 2) {}
};

struct Texture {//guarda el path de la textura i l'id

	std::string path, id;
	
	Texture(std::string _path, std::string _id) : path(_path), id(_id) {};
};


struct AnimatedTexture {//guarda el path de la textura i l'id

	std::string path, id;
	int textWidth, textHeight, frameWidth, frameHeight;

	AnimatedTexture(std::string _path, std::string _id, int _textWidth, int _textHeight, int _frameWidth, int _frameHeight) : path(_path), id(_id), textWidth(_textWidth), textHeight(_textHeight), frameWidth(_frameWidth), frameHeight(_frameHeight){};

};
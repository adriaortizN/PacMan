#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string.h>
#include <unordered_map>
#include "Types.h"
#include "Constants.h"
#include <SDL_mixer.h>
#include <exception>


class Renderer {

private:
	SDL_Renderer *m_renderer = nullptr;
	SDL_Window *m_window = nullptr;
	std::unordered_map<std::string, SDL_Texture*> m_textureData;
	std::unordered_map<std::string, TTF_Font*> m_fontData;
	static Renderer *renderer;
	Renderer();

public:
	static Renderer *Instance()
	{
		if (renderer == nullptr) { renderer = new Renderer; }
		return renderer;
	};

	~Renderer();
	 void Clear()  ;
	 void Render() ;
	 void LoadFont(Font font);
	 void LoadTexture(const std::string &id, const std::string &path);
	 void LoadTextureText(const std::string &fontId, Text text);
	 Vector2 GetTextureSize(const std::string &id);
	 void PushImage(const std::string &id,const Rect &rect);
	 void PushSprite(const std::string &id,const Rect &rectSprite,const Rect &rectPos);
	 void PushRotatedSprite(const std::string &id, const Rect &rectSprite, const Rect &rectPos, float angle);
	 void SetRendreDrawColor(int r, int g, int b);	


};



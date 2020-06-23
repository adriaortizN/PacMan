#include "Renderer.h"
#include <iostream>
Renderer::Renderer()
{
	// --- INIT ---
	try
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw std::exception("No es pot inicialitzar SDL subsystems");
	}
	catch(std::exception e)
	{
		std::cout << e.what() << '\n';
	}
	// --- WINDOW ---

	try 
	{
		m_window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (m_window == nullptr) throw std::exception("No es pot inicialitzar SDL_Window");
	}
	catch (std::exception e) 
	{
		std::cout << e.what() << '\n';
	}

	// --- RENDERER ---
	try 
	{
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (m_renderer == nullptr) throw std::exception("No es pot inicialitzar SDL_Renderer");
	}
	catch (std::exception e) 
	{
		std::cout << e.what() << '\n';
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	//Initialize PNG loading
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	try 
	{
		if (!(IMG_Init(imgFlags) & imgFlags)) throw std::exception("Error: SDL_imageinit");
	}
	catch (std::exception e) 
	{
		std::cout << e.what() << '\n';
	}

	// ---- TTF ----
	try 
	{
		if (TTF_Init() != 0) throw std::exception("No es pot inicialitzar SDL_ttf");
	}
	catch (std::exception e) 
	{
		std::cout << e.what() << '\n';
	}

	// ICON IMAGE
	SDL_Surface* tmpSurface{ };
	SDL_SetWindowIcon(m_window, tmpSurface);
	SDL_FreeSurface(tmpSurface);
};


Renderer::~Renderer()
{
	

	for (auto &t : m_textureData) SDL_DestroyTexture(t.second), t.second = nullptr;
	for (auto &f : m_fontData) TTF_CloseFont(f.second), f.second = nullptr;
	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;
	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

};

void Renderer::Clear() { SDL_RenderClear(m_renderer); };

void Renderer::Render() { SDL_RenderPresent(m_renderer); };

void Renderer::LoadFont(Font font)
{
	if (m_fontData.find(font.id) != m_fontData.end())
	{
		return;
	}
	//std::cout << font.id << std::endl;
	TTF_Font *ttfFont{ TTF_OpenFont(font.path.c_str(), font.size) };
	try
	{
		if (ttfFont == nullptr) throw std::exception("No espot inicialitzar TTF_Font");
	}
	catch (std::exception e) {
		std::cout << e.what() << '\n';
	}
	m_fontData[font.id] = ttfFont;
};

void Renderer::LoadTexture(const std::string &id, const std::string &path)
{
	if (m_textureData.find(id) != m_textureData.end())
	{
		return;
	}
	//std::cout << id << std::endl;
	SDL_Texture *texture = { IMG_LoadTexture(m_renderer, path.c_str()) };
	try 
	{
		if (texture == nullptr) throw std::exception("No s'ha pogut carregar la textura");
	}
	catch (std::exception e)
	{
		std::cout << e.what() << '\n';
	}

	m_textureData[id] = texture;
};

void Renderer::LoadTextureText(const std::string &fontId, Text text) 
{
	
	SDL_Surface	*tmpSurf{ TTF_RenderText_Blended(m_fontData[fontId], text.text.c_str(), SDL_Color{ text.color.r, text.color.g, text.color.b,text.color.a }) };
	try
	{
		tmpSurf = { TTF_RenderText_Blended(m_fontData[fontId], text.text.c_str(), SDL_Color{ text.color.r, text.color.g, text.color.b,text.color.a }) };
		if (tmpSurf == nullptr) throw std::exception("Unable to create the SDL text surface");
	}
	catch (const std::exception e)
	{
		std::cout << e.what() << '\n';
	}
	SDL_Texture *texture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
	try
	{
		if (texture == nullptr) throw  std::exception("Unable to create Texture from a Surface");
	}
	catch (const std::exception e)
	{
		std::cout << e.what() << '\n';
	}
	
	m_textureData[text.id] = texture;
	SDL_FreeSurface(tmpSurf);
	tmpSurf = NULL;
};

Vector2 Renderer::GetTextureSize(const std::string &id) 
{
	int w; int h;
	SDL_QueryTexture(m_textureData[id], NULL, NULL,&w, &h);
	return {w, h};
};

void Renderer::PushImage(const std::string &id, const Rect &_rect) 
{
	SDL_Rect rect{ _rect.x, _rect.y, _rect.w, _rect.h };
	SDL_RenderCopy(m_renderer, m_textureData[id], nullptr, &rect);
};

void Renderer::PushSprite(const std::string &id, const Rect &_rectSprite,const Rect &_rectPos) 
{
	if (&_rectSprite != nullptr) {
		SDL_Rect rectSprite{ _rectSprite.x, _rectSprite.y, _rectSprite.w, _rectSprite.h };
		SDL_Rect rectPos{ _rectPos.x, _rectPos.y, _rectPos.w, _rectPos.h };
		SDL_RenderCopy(m_renderer, m_textureData[id], &rectSprite, &rectPos);
	}
	
}

void Renderer::PushRotatedSprite(const std::string & id, const Rect & _rectSprite, const Rect & _rectPos, float angle)
{
	//std::cout << "RECT: " << _rectSprite.h << " " << _rectSprite.w << " " << _rectSprite.x << " " << _rectSprite.y << " " << std::endl;
	SDL_Rect rectSprite{ _rectSprite.x, _rectSprite.y, _rectSprite.w, _rectSprite.h };
	SDL_Rect rectPos{ _rectPos.x, _rectPos.y, _rectPos.w, _rectPos.h };
	SDL_Point center = { 10, 10 };
	SDL_RenderCopyEx(m_renderer, m_textureData[id], &rectSprite, &rectPos, angle, &center, SDL_FLIP_NONE);
}

void Renderer::SetRendreDrawColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
}

Renderer* Renderer::renderer = nullptr;

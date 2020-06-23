#include "TextTexture.h"



TextTexture::TextTexture(Text _text, Vector2 pos, Font _font, ALIGN _align ) : text(_text), rect({pos.x, pos.y, 0, 0}), font(_font)
{
	Renderer * render = Renderer::Instance();
	render->LoadFont(_font);
	render->LoadTextureText(_font.id, text);
	Vector2 size = render->GetTextureSize(text.id);
	rect.w = size.x;
	rect.h = size.y;
	switch (_align)
	{
	case ALIGN::CENTER:
		rect.x -= rect.w/2;
		break;
	case ALIGN::RIGHT:
		rect.x -= rect.w;
		break;
	case ALIGN::LEFT:
	default:
		break;
	}
}

void TextTexture::UpdateText(std::string _text) 
{
	text.text = _text;
	Renderer * rend = Renderer::Instance();
	rend->LoadTextureText(font.id, text);
	Vector2 size = rend->GetTextureSize(text.id);
	rect.w = size.x;
	rect.h = size.y;
}

std::string TextTexture::GetText() { return text.text; }

void TextTexture::Draw() { Renderer::Instance()->PushImage(text.id, rect); }

Rect TextTexture::GetRect() { 
	return rect;
}

TextTexture::~TextTexture()
{
}

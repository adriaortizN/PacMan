#include "Button.h"



Button::Button(std::string _text, Vector2 pos) :
	defaultText(new TextTexture{ {_text + " Button", _text, COLOR_WHITE}, {pos.x, pos.y}, DEFAULT_FONT, ALIGN::LEFT }),
	hoverText(new TextTexture{ {"Hover " + _text + " Button", _text, COLOR_RED}, {pos.x, pos.y}, DEFAULT_FONT, ALIGN::LEFT }),
	pressed(false) 
{ 
	text = defaultText;
}

void Button::Update(InputStruct inputs) {
	bool colliding;
	isColliding(inputs.mousePos, text->GetRect()) ? (text = hoverText, colliding = true) : (text = defaultText, colliding = false); //si el ratoli esta fent hover retorna true i canvia el text

	(colliding && inputs.mouse && !inputs.lastMouse) ? pressed = true : pressed = false; //si esta sent presionat o no
}

void Button::Draw() { text->Draw(); } //draw

bool Button::IsPressed() { return pressed; } //retorna si esta presionat o no

void Button::UpdateText(std::string name) {
	defaultText->UpdateText(name);
	hoverText->UpdateText(name);
}

Button::~Button()
{
}

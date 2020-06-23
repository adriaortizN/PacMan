#pragma once
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <exception>

class Mixer
{
public:
	Mixer(std::string, int);
	~Mixer();
	bool ToggleAudio();
	void PlaySound(std::string);
};


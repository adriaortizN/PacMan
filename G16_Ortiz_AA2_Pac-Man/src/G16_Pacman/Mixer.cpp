#include "Mixer.h"



Mixer::Mixer(std::string path, int freq = MIX_DEFAULT_FREQUENCY)
{
	//init mixer
	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };

	try {
		if (!(Mix_Init(mixFlags) & mixFlags)) throw std::exception("No s'ha inicialitzat SDL_mix");
	}
	catch (std::exception e) {
		std::cout << e.what() << '\n';
	}

	try {
		if (Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, 2, 1024) == -1) throw std::exception("Unable to initialize SDL_mixer audio system");
	}
	catch (std::exception e) {
		std::cout << e.what() << '\n';
	}

	if (path != "")
	{
		Mix_Music *soundtrack{ Mix_LoadMUS(path.c_str()) };

		try {
			if (!soundtrack) throw std::exception("Unable to load the Mix_Music soundtrack");
		}
		catch (std::exception e) {
			std::cout << e.what() << '\n';
		}

		Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
		Mix_PlayMusic(soundtrack, -1);
	}
}

void Mixer::PlaySound(std::string path) 
{
	Mix_Chunk *sound = Mix_LoadWAV(path.c_str());
	Mix_PlayChannel(-1, sound, 0 );

	sound = NULL;
}

bool Mixer::ToggleAudio() 
{
	if (!Mix_PausedMusic())
	{
		Mix_PauseMusic();
		return true;
	}
	else 
	{
		Mix_ResumeMusic();
		return false;
	}
	
}

Mixer::~Mixer()
{
	Mix_CloseAudio();
	Mix_Quit();
}

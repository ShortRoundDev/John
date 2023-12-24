#pragma once
#include "SDL2/SDL.h"

#include <string>

class Texture
{
public:
	_Success_(return);
	static bool tryLoad(_In_ std::string path, _Out_ Texture** texture);
	
	SDL_Texture* texture;
	SDL_Rect src;
private:
	Texture(SDL_Texture* texture);
	~Texture();
};


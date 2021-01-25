#pragma once

#include "Game.hpp"
#include <string>
#include <iostream>
#include <inttypes.h>

using namespace std;

class TextureManager {

public:
	static SDL_Texture* loadTexture(const char* fileName);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
};
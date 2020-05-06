#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include <math.h>

class TextImageComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;
	int imageScale;

	TextImageComponent() = default;

	~TextImageComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TextImageComponent(int srcX, int srcY, int xpos, int ypos, const char* path)
	{
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		texture = TextureManager::loadTexture(path);
		position.x = xpos;
		position.y = ypos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = 265;
		srcRect.h = 600;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = 132;
		destRect.h = 300;
	}

	void update() override
	{
		float trueScale = 1.032712419896443;

		destRect.w = 132 * pow(trueScale, abs(Game::scale));
		destRect.h = 300 * pow(trueScale, abs(Game::scale));

		destRect.x = position.x * pow(trueScale, abs(Game::scale)) - Game::camera.x;
		destRect.y = position.y * pow(trueScale, abs(Game::scale)) - Game::camera.y;

		if (Game::scale <= 20)
		{
			SDL_SetTextureAlphaMod(texture, 200 - 10 * Game::scale);
		}
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};
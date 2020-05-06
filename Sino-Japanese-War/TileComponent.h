#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include <math.h>

class TileComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D index;
	int tileScale;
	int tileType;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int xindex, int yindex, float scale, int type, const char* path)
	{
		tileScale = scale;
		index.x = xindex;
		index.y = yindex;
		tileType = type;

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		texture = TextureManager::loadTexture(path);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = 60 * scale;
		srcRect.h = 52 * scale;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = 60;
		destRect.h = 52;
	}

	bool inTile(float inputX, float inputY)
	{
		float x = inputX - destRect.x;
		float y = destRect.h - (inputY - destRect.y);
		float k = 26. / 15.;
		if (-k * x + 0.5 * destRect.h <= y && k * x + 0.5 * destRect.h >= y &&
			(1/k) * y + 0.75 * destRect.w >= x && -(1/k) * y + 1.25 * destRect.w >= x &&
			y <= destRect.h && y >= 0)
		{
			return true;
		}
		return false;
	}

	void update() override
	{
		float trueScale = 1.032712419896443;

		destRect.w = 60 * pow(trueScale, abs(Game::scale));
		destRect.h = 52 * pow(trueScale, abs(Game::scale));

		int xpos = 1.5 * destRect.w * index.x / 2;
		int ypos = destRect.h * index.y;
		if (int(index.x) % 2 == 1)
		{
			xpos = 1.5 * destRect.w * (index.x - 1) / 2 + 0.75 * destRect.w;
			ypos += 0.5 * destRect.h;
		}

		destRect.x = xpos - Game::camera.x;
		destRect.y = ypos - Game::camera.y;

		if (Game::scale <= 20 && tileType == 1)
		{
			SDL_SetTextureAlphaMod(texture, 5 * Game::scale);	
		}
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};
#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include <math.h>

class WarshipComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect, hexRect;
	Vector2D position;
	Vector2D movPosition;
	//Navigation nav;

	bool isChosen;
	bool isMoving;

	int country;
	int shipType;

	WarshipComponent() = default;

	~WarshipComponent()
	{
		SDL_DestroyTexture(texture);
	}

	WarshipComponent(int srcX, int srcY, int xindex, int yindex, int country, int type, float scale, const char* path)
	{

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		texture = TextureManager::loadTexture(path);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = 400 * scale;
		srcRect.h = 400 * scale;

		position.x = xindex;
		position.y = yindex;

		hexRect.w = 60;
		hexRect.h = 52;

		int xpos = 1.5 * hexRect.w * position.x / 2;
		int ypos = hexRect.h * position.y;
		if (int(position.x) % 2 == 1)
		{
			xpos = 1.5 * hexRect.w * (position.x - 1) / 2 + 0.75 * hexRect.w;
			ypos += 0.5 * hexRect.h;
		}

		destRect.x = xpos + 10;
		destRect.y = ypos + 6;
		destRect.w = 40;
		destRect.h = 40;
	}
	void Move(int x, int y)
	{
		isMoving = true;
		movPosition.x = x;
		movPosition.y = y;
	}

	void offClicked()
	{
		isChosen = false;
		texture = TextureManager::loadTexture("../assets/ship.png");
	}

	void onClicked()
	{
		isChosen = true;
		texture = TextureManager::loadTexture("../assets/shipischosen.png");
	}

	void update() override
	{
		float trueScale = 1.032712419896443;

		destRect.w = 40 * pow(trueScale, abs(Game::scale));
		destRect.h = 40 * pow(trueScale, abs(Game::scale));

		hexRect.w = 60 * pow(trueScale, abs(Game::scale));
		hexRect.h = 52 * pow(trueScale, abs(Game::scale));
		int xpos = 1.5 * hexRect.w * position.x / 2;
		int ypos = hexRect.h * position.y;
		if (int(position.x) % 2 == 1)
		{
			xpos = 1.5 * hexRect.w * (position.x - 1) / 2 + 0.75 * hexRect.w;
			ypos += 0.5 * hexRect.h;
		}

		destRect.x = xpos - Game::camera.x + 10 * pow(trueScale, abs(Game::scale));
		destRect.y = ypos - Game::camera.y + 6 * pow(trueScale, abs(Game::scale));

		if (isMoving)
		{
			position.x = movPosition.x;
			position.y = movPosition.y;
		}

		if (Game::scale <= 20 && Game::scale >= 10)
		{
			SDL_SetTextureAlphaMod(texture, 255 * Game::scale / 20);	
		}
		else if (Game::scale < 10)
		{
			SDL_SetTextureAlphaMod(texture, 0);
		}
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};

/*
class Navigation
{
	
};*/
#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include "WarshipData.h"
#include <math.h>

class WarshipSelectorComponent : public Component
{
public:
	SDL_Texture* texture;
	const char* texturePath0;
	const char* texturePath1;
	string name;
	WarshipData *data;
	bool isChosen;
	SDL_Rect srcRect, destRect, textSrcRect, textDestRect;
	SDL_Texture* tex;

	WarshipSelectorComponent() = default;

	~WarshipSelectorComponent()
	{
		SDL_DestroyTexture(texture);
	}

	WarshipSelectorComponent(int x, int y, float scale, const char* notChosenImgPath, const char* chosenImgPath, WarshipData *warshipData)
	{
		destRect.x = x;
		destRect.y = y;
		destRect.w = 300;
		destRect.h = 50;
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = 300;
		srcRect.h = 50;

		textDestRect.x = x + 160;
		textDestRect.y = y + 5;
		textDestRect.w = 60;
		textDestRect.h = 30;
		textSrcRect.x = 0;
		textSrcRect.y = 0;
		textSrcRect.w = 60;
		textSrcRect.h = 30;

		texturePath0 = notChosenImgPath;
		texturePath1 = chosenImgPath;
		data = warshipData;
		name = warshipData->name;
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		texture = TextureManager::loadTexture(texturePath0);

		TTF_Font* font = TTF_OpenFont("../assets/font/STFANGSO.TTF", 27);
		//TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		SDL_Color color = { 0, 0, 0 };
		SDL_Surface* surf = TTF_RenderUTF8_Solid(font, name.c_str(), color);
		tex = SDL_CreateTextureFromSurface(Game::renderer, surf);
	}

	void offClicked()
	{
		isChosen = false;
		texture = TextureManager::loadTexture(texturePath0);
	}

	void onClicked()
	{
		isChosen = true;
		texture = TextureManager::loadTexture(texturePath1);
	}

	void update() override
	{
		// TODO：根据舰船状态改变
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
		SDL_RenderCopyEx(Game::renderer, tex, &textSrcRect, &textDestRect, NULL, NULL, SDL_FLIP_NONE);
	}

	BOOL clickCheck(int x, int y) override
	{
		if (x >= destRect.x &&
			x <= destRect.x + destRect.w &&
			y >= destRect.y &&
			y <= destRect.y + destRect.h)
		{
			return TRUE;
		}
		return FALSE;
	}
};
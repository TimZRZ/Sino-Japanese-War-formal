#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "WarshipData.h"
#include "SDL.h"
#include <math.h>

class WarshipComponent : public Component
{
public:
	SDL_Texture* texture0;
	SDL_Texture* texture1;
	WarshipData* data;
	SDL_Rect srcRect, destRect, hexRect;
	Vector2D movPosition;

	// ʵ�ֶ�㿨Ƭ
	int panelLevel;
	BOOL isTop;

	bool isChosen;
	bool isMoving;

	WarshipComponent() = default;

	~WarshipComponent()
	{
		SDL_DestroyTexture(texture0);
		SDL_DestroyTexture(texture1);
	}

	WarshipComponent(int srcX, int srcY, int xindex, int yindex, float scale, const char* path, int level)
	{
		isChosen = false;
		data = new WarshipData();
		SDL_SetTextureBlendMode(texture0, SDL_BLENDMODE_BLEND);
		SDL_SetTextureBlendMode(texture1, SDL_BLENDMODE_BLEND);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = 400 * scale;
		srcRect.h = 400 * scale;

		data->position.x = xindex;
		data->position.y = yindex;

		//vector<WarshipComponent *> levelVector = Game::warshipStack[int(data->position.x)][int(data->position.y)];
		//vector<WarshipComponent *>::iterator it = find(levelVector.begin(), levelVector.end(), this);

		panelLevel = level;

		hexRect.w = 60;
		hexRect.h = 52;

		int xpos = 1.5 * hexRect.w * data->position.x / 2;
		int ypos = hexRect.h * data->position.y;
		if (int(data->position.x) % 2 == 1)
		{
			xpos = 1.5 * hexRect.w * (data->position.x - 1) / 2 + 0.75 * hexRect.w;
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
		
	}

	void onClicked()
	{
		isChosen = true;
	}

	void update() override
	{
		float trueScale = 1.032712419896443;

		destRect.w = 40 * pow(trueScale, abs(Game::scale));
		destRect.h = 40 * pow(trueScale, abs(Game::scale));

		hexRect.w = 60 * pow(trueScale, abs(Game::scale));
		hexRect.h = 52 * pow(trueScale, abs(Game::scale));
		int xpos = 1.5 * hexRect.w * data->position.x / 2;
		int ypos = hexRect.h * data->position.y;
		if (int(data->position.x) % 2 == 1)
		{
			xpos = 1.5 * hexRect.w * (data->position.x - 1) / 2 + 0.75 * hexRect.w;
			ypos += 0.5 * hexRect.h;
		}

		destRect.x = xpos - Game::camera.x + (10 + panelLevel) * pow(trueScale, abs(Game::scale));
		destRect.y = ypos - Game::camera.y + (6 - panelLevel + ((isChosen&&!isTop)?20:0)) * pow(trueScale, abs(Game::scale));

		if (isMoving)
		{
			data->position.x = movPosition.x;
			data->position.y = movPosition.y;
		}

		if (Game::scale <= 20 && Game::scale >= 10)
		{
			SDL_SetTextureAlphaMod(texture0, 255 * Game::scale / 20);
			SDL_SetTextureAlphaMod(texture1, 255 * Game::scale / 20);
		}
		else if (Game::scale < 10)
		{
			SDL_SetTextureAlphaMod(texture0, 0);
			SDL_SetTextureAlphaMod(texture1, 0);
		}
	}

	void draw() override
	{
		TextureManager::Draw((isChosen? texture1:texture0), srcRect, destRect, SDL_FLIP_NONE);
	}

	BOOL clickCheck(int x, int y) override
	{
		cout << "-------" << endl;
		cout << data->id << endl;
		cout << destRect.x << ", "<< x << ", "<< destRect.x + destRect.w << endl;
		cout << destRect.y << ", "<< y << ", " << destRect.y + destRect.h << endl;
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
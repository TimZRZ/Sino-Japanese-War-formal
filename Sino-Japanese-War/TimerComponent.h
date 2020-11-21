#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include <math.h>
#include <SDL_ttf.h>

using namespace std;

class TimerComponent : public Component
{
public:
	int turnNum;
	int day, month, year;

	SDL_Texture* texture;
	SDL_Texture* text_texture;

	SDL_Rect srcRect, destRect;
	SDL_Rect textRect;

	SDL_Surface* text_surface;
	SDL_Color textColor = { 255, 255, 255, 0 };
	TTF_Font* Sans = TTF_OpenFont("../assets/times.ttf", 24);

	char *localeToUTF8(const char *src) {
		static char *buf = NULL;
		if (buf)
		{
			free(buf);
			buf = NULL;
		}
		wchar_t *unicode_buf;
		int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		unicode_buf = (wchar_t*)malloc((nRetLen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);
		nRetLen = WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
		buf = (char*)malloc(nRetLen + 1);
		WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
		free(unicode_buf);
		return buf;
	}

	TimerComponent()
	{
		day = 1;
		month = 7;
		year = 1897L;

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		texture = TextureManager::loadTexture("../assets/timer.png");

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = 400;
		srcRect.h = 200;

		destRect.x = 1520;
		destRect.y = 0;
		destRect.w = 400;
		destRect.h = 200;


		TTF_Init();
		TTF_Font *font = TTF_OpenFont("../assets/mingliub.ttc", 24);
		if (!font) {
			MessageBox(0, 0, "no", 0);
		}

		char timeStamp[20];
		sprintf_s(timeStamp, "%d%s%d%s%d%s", (char *)year, "YEAR", (char *)month, "MONTH", (char *)day, "DAY");

		text_surface = TTF_RenderUTF8_Solid(font, localeToUTF8(timeStamp), textColor);
		text_texture = SDL_CreateTextureFromSurface(Game::renderer, text_surface);

		textRect.x = 1540;
		textRect.y = 30;
		textRect.w = text_surface->w;
		textRect.h = text_surface->h;
	}

	~TimerComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void update() override
	{
		/*
		text_surface = TTF_RenderUTF8_Solid(Sans, (
			std::to_string(year) + localeToUTF8("Äê") +
			std::to_string(month) + localeToUTF8("ÔÂ") +
			std::to_string(day) + localeToUTF8("ÈÕ")).c_str(), textColor);
		textRect.w = text_surface->w;
		textRect.h = text_surface->h;
		text_texture = SDL_CreateTextureFromSurface(Game::renderer, text_surface);
		SDL_RenderCopy(Game::renderer, text_texture, NULL, &textRect);*/
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
		SDL_RenderCopy(Game::renderer, text_texture, NULL, &textRect);
	}
};
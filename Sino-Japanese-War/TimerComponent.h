#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"
#include <math.h>
#include <SDL_ttf.h>

class TimerComponent : public Component
{
public:
	int mStartTicks, mPausedTicks, lastStartTicks;
	int day, month;
	bool mStarted, mPaused;

	SDL_Texture* texture;
	SDL_Texture* text_texture;

	SDL_Rect srcRect, destRect;
	SDL_Rect textRect;

	SDL_Surface* text_surface;
	SDL_Color textColor = { 255, 255, 255, 0 };
	TTF_Font* Sans = TTF_OpenFont("../assets/times.ttf", 24);

	TimerComponent()
	{
		day = 1;
		month = 1;

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

		text_surface = TTF_RenderText_Solid(Sans, ("Month: " + std::to_string(month) + " Day: " + std::to_string(day) + " Time: " + std::to_string(mStartTicks)).c_str(), textColor);
		text_texture = SDL_CreateTextureFromSurface(Game::renderer, text_surface);

		textRect.x = 1540;
		textRect.y = 30;
		textRect.w = text_surface->w;
		textRect.h = text_surface->h;
		Start();
	}

	~TimerComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void Start()
	{
		mStarted = true;
		mPaused = false;
		//while (true)
		//{
		SDL_Init(SDL_INIT_TIMER);
		mStartTicks = SDL_GetTicks() / 1000;
		//}
		
		
	}

	void Stop()
	{
		mStarted = false;
		mPaused = false;
	}

	void Pause()
	{
		if (mStarted && !mPaused) {
			mPaused = true;
			mPausedTicks = SDL_GetTicks() - mStartTicks;
		}
	}

	void Unpause()
	{
		if (mPaused) {
			mPaused = false;
			mStartTicks = SDL_GetTicks() - mPausedTicks;
			mPausedTicks = 0;
		}
	}

	int Restart()
	{
		int elapsedTicks = Ticks();
		Start();
		return elapsedTicks;
	}

	int Ticks() const
	{
		if (mStarted) {
			if (mPaused)
				return mPausedTicks;
			else
				return SDL_GetTicks() / 1000 - mStartTicks;
		}
		return 0;
	}

	bool Started() const
	{
		return mStarted;
	}

	bool Paused() const
	{
		return mPaused;
	}

	void update() override
	{
		mStartTicks = (SDL_GetTicks() / 1000) % 24;
		if (mStartTicks == 0 && mStartTicks != lastStartTicks)
		{
			day += 1;
		}
		if (day == 31)
		{
			month += 1;
			day = 1;
		}
		text_surface = TTF_RenderText_Solid(Sans, ("Month: " + std::to_string(month) + " Day: " + std::to_string(day) + " Time: " + std::to_string(mStartTicks)).c_str(), textColor);
		textRect.w = text_surface->w;
		textRect.h = text_surface->h;
		text_texture = SDL_CreateTextureFromSurface(Game::renderer, text_surface);
		SDL_RenderCopy(Game::renderer, text_texture, NULL, &textRect);
		lastStartTicks = mStartTicks;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
		SDL_RenderCopy(Game::renderer, text_texture, NULL, &textRect);
	}
};
#ifndef Game_hpp
#define Game_hpp

#include "SDL.h"
#include "SDL_image.h"
#include "Vector2D.h"
#include <iostream>
#include <vector>

class ColliderComponent;

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos,
		int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();

	static bool dragMap;
	static int scale;

	static void addTile(int srcX, int srcY, int xpos, int ypos, int xindex, int yindex);
	static void addBackGroundTile(int srcX, int srcY, int xpos, int ypos, int xindex, int yindex);
	static void addTextImage(int srcX, int srcY, int xpos, int ypos, const char* path);

	static void checkClick(int x, int y);
	static void checkMove(int x, int y);

	static SDL_Renderer *renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	bool running() { return isRunning; };
	static bool isRunning;

	static SDL_Rect camera;
	static Vector2D posMap00;
	static Vector2D posTile00;

private:
	int cnt = 0;
	SDL_Window *window;

};

#endif
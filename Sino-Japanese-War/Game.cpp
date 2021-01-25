#include "Game.hpp"
#include "Types.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "Warships.h"
#include "WarshipSelectorComponent.h"

#define LEFT_OFFSET -75
#define SCALE 1.032712419896443

using namespace std;

Map* map;
Manager manager;
Warships *warshipManager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int screenW = 1920;
int screenH = 1060;
SDL_Rect Game::camera = { LEFT_OFFSET,0,screenW,screenH };

Vector2D Game::posMap00 = Vector2D(0, 0);
Vector2D Game::posTile00 = Vector2D(0, 0);

std::vector<ColliderComponent*> Game::colliders;
bool Game::isRunning = false;
bool Game::dragMap = false;

auto& chosenShip(manager.addEntity());

//auto& timer(manager.addEntity());

int Game::scale = 0;

const char* mapfile = "../assets/terrain_ss.png";

auto& mapImages(manager.getGroup(groupMapImage));
auto& mapTexts(manager.getGroup(groupMapText));
auto& tiles(manager.getGroup(groupMap));
auto& warships(manager.getGroup(groupWarships));
auto& enemies(manager.getGroup(groupEnemies));
auto& warshipSelectors(manager.getGroup(groupWarshipSelectors));

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos,
	int width, int height, bool fullscreen)
{
	TTF_Init();
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "Subsystems Initialized! ..." << endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			cout << "Window Created!" << endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 225, 204, 161, 255);
			cout << "Renderer Created!" << endl;
		}
		isRunning = true;
	} else {
		isRunning = false;
	}

	//ECS implementation

	//timer.addComponenet<TimerComponent>();

	Map::loadMap("../assets/map.map", 40, 20);

	warshipManager = new Warships();
	warshipManager->loadWarships(&manager);

	// Selector相关
	int selectorY = 50;
	for (auto& s : warships)
	{
		const char* notChosenImgPath;
		const char* chosenImgPath;
		switch (s->getComponent<WarshipComponent>().data->type) {
			case 1:
				notChosenImgPath = "../assets/selector/q1-0.png";
				chosenImgPath = "../assets/selector/q1-1.png";
				break;
			case 2:
				notChosenImgPath = "../assets/selector/q2-0.png";
				chosenImgPath = "../assets/selector/q2-1.png";
				break;
			case 3:
				notChosenImgPath = "../assets/selector/q3-0.png";
				chosenImgPath = "../assets/selector/q3-1.png";
				break;
			case 4:
				notChosenImgPath = "../assets/selector/q3-0.png";
				chosenImgPath = "../assets/selector/q3-1.png";
				break;
		}

		auto& selector(manager.addEntity());
		selector.addComponenet<WarshipSelectorComponent>(0, selectorY, 1, notChosenImgPath, chosenImgPath, s->getComponent<WarshipComponent>().data);
		selector.addGroup(groupWarshipSelectors);
		selectorY += 50;
	}
}

int mouseX;
int mouseY;
float trueScale = 1.032712419896443;

void Game::checkClick(int x, int y)
{
	BOOL shouldReturn = FALSE;
	for (auto& s : warshipSelectors)
	{
		if (s->getComponent<WarshipSelectorComponent>().clickCheck(x, y))
		{
			s->getComponent<WarshipSelectorComponent>().onClicked();
			warshipManager->choseWarship(s->getComponent<WarshipSelectorComponent>().data->position.x,
										s->getComponent<WarshipSelectorComponent>().data->position.y,
										s->getComponent<WarshipSelectorComponent>().data->id);
			shouldReturn = TRUE;
		}
		else {
			if (s->getComponent<WarshipSelectorComponent>().isChosen)
			{
				s->getComponent<WarshipSelectorComponent>().offClicked();
			}
		}
	}

	if (shouldReturn)
	{
		return;
	}

	for (auto& s : warships)
	{
		if (s->getComponent<WarshipComponent>().clickCheck(x, y))
		{
			int indexX = int(s->getComponent<WarshipComponent>().data->position.x);
			int indexY = int(s->getComponent<WarshipComponent>().data->position.y);
			warshipManager->checkNext(indexX, indexY);

			for (auto& s : warshipSelectors) 
			{
				string id = warshipManager->warshipStack[indexX][indexY][warshipManager->curIndex]->data->id;
				if (s->getComponent<WarshipSelectorComponent>().data->id == id)
				{
					s->getComponent<WarshipSelectorComponent>().onClicked();
				}
				else 
				{
					s->getComponent<WarshipSelectorComponent>().offClicked();
				}
	
			}

			break;
		}
	}

	event.type = SDL_MOUSEBUTTONUP;
}

void Game::checkMove(int x, int y)
{
	for (auto& t : tiles)
	{
		if (t->getComponent<TileComponent>().inTile(x, y))
		{
			for (auto& s : warships)
			{
				if (s->getComponent<WarshipComponent>().isChosen)
				{
					s->getComponent<WarshipComponent>().Move(t->getComponent<TileComponent>().index.x, t->getComponent<TileComponent>().index.y);
				}

			}
		}
	}
}

void Game::handleEvents()
{
	
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				checkClick(event.motion.x, event.motion.y);
			if (event.button.button == SDL_BUTTON_RIGHT)
				checkMove(event.motion.x, event.motion.y);
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_MOUSEWHEEL:
			SDL_GetMouseState(&mouseX, &mouseY);
			if (event.wheel.y == 1) // scroll up
			{
				// Put code for handling "scroll up" here!
				if (scale < 50)
				{
					/* Todo: 日后需要改进*/
					//计算camera中点
					int centerX = camera.x + (camera.w / 2) * pow(SCALE, scale);
					int centerY = camera.y + (camera.h / 2) * pow(SCALE, scale);
					int neoCenterX = camera.x + (camera.w / 2) * pow(SCALE, scale + 1);
					int neoCenterY = camera.y + (camera.h / 2) * pow(SCALE, scale + 1);
					camera.x += neoCenterX - centerX;
					camera.y += neoCenterY - centerY;
					//cout << camera.x << " , " << camera.y << endl;
					scale += 1;
				}
				else {
					scale = 50;
				}
			}
			else if (event.wheel.y == -1) // scroll down
			{
				if (scale > 0)
				{
					//计算camera中点
					int centerX = camera.x + (camera.w / 2) * pow(SCALE, scale);
					int centerY = camera.y + (camera.h / 2) * pow(SCALE, scale);
					int neoCenterX = camera.x + (camera.w / 2) * pow(SCALE, scale - 1);
					int neoCenterY = camera.y + (camera.h / 2) * pow(SCALE, scale - 1);
					int XOffSet = (camera.x + neoCenterX - centerX >= LEFT_OFFSET * pow(SCALE, scale - 1)) ?  neoCenterX - centerX : LEFT_OFFSET - camera.x;
					int YOffSet = (camera.y + neoCenterY - centerY >= 0) ? neoCenterY - centerY : 0 - camera.y;

					camera.x = (camera.x + camera.w <= (screenW + LEFT_OFFSET) * pow(SCALE, scale - 1)) ? camera.x + XOffSet : (screenW + LEFT_OFFSET) * pow(SCALE, scale - 1) - camera.w;
					camera.y = (camera.y + camera.h <= screenH * pow(SCALE, scale - 1)) ? camera.y + YOffSet : screenH * pow(SCALE, scale - 1) - camera.h;				
					scale -= 1;
				}
				else
				{
					scale = 0;
				}
			}
			event.type = SDL_MOUSEWHEEL_NORMAL;
			break;
		default:
			break;
	}
}


void Game::update()
{
	manager.refresh();
	manager.update();

	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX < 50 && camera.x > LEFT_OFFSET)
	{
		camera.x -= 10;
	}
	else if (mouseX > screenW - 50 && camera.x + screenW - LEFT_OFFSET < screenW * pow(trueScale, abs(Game::scale)))
	{
		camera.x += 10;
	}

	if (mouseY < 50 && camera.y > 0)
	{
		camera.y -= 10;
	}
	else if (mouseY > screenH - 50 && camera.y + screenH < screenH * pow(trueScale, abs(Game::scale)))
	{
		camera.y += 10;
	}

	/*
	cout << mouseX << ", " << mouseY << endl;

	if (dragMap)
	{
		if (mouseX >= 0)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			if (abs(mouseX - x) < 20 && abs(mouseY - y) < 20)
			{
				camera.x += mouseX - x;
				camera.y += mouseY - y;
			}
			mouseX = x;
			mouseY = y;

			if (Game::event.type == SDL_MOUSEBUTTONUP)
			{
				dragMap = false;
			}
		}
		
	}
	else if (Game::event.type == SDL_MOUSEBUTTONDOWN)
	{
		dragMap = true;
		SDL_GetMouseState(&mouseX, &mouseY);
	}
	*/
}

void Game::render()
{
	SDL_RenderClear(renderer);


	
	for (auto& i : mapImages)
	{
		i->draw();
	}
	
	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& s : warships)
	{
		s->draw();
	}

	for (auto& k : mapTexts)
	{
		k->draw();
	}

	for (auto& s : warshipSelectors)
	{
		s->draw();
	}

	//timer.draw();

	/*
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}*/

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game Cleaned" << endl;
}

void Game::addTile(int srcX, int srcY, int xpos, int ypos, int xindex, int yindex, int xTrans, int yTrans)
{
	auto& tile(manager.addEntity());
	tile.addComponenet<TileComponent>(srcX, srcY, xpos, ypos, xindex, yindex, 6, 1, "../assets/hexTile.png", 0, 0);
	tile.addGroup(groupMap);
}

void Game::addBackGroundTile(int srcX, int srcY, int xpos, int ypos, int xindex, int yindex, int xTrans, int yTrans)
{
	auto& tile(manager.addEntity());
	tile.addComponenet<TileComponent>(srcX, srcY, xpos, ypos, xindex, yindex, 0.5, 0, "../assets/map_small2.png", xTrans, yTrans);
	tile.addGroup(groupMapImage);
}

void Game::addStringTile(int srcX, int srcY, int xpos, int ypos, int xindex, int yindex, const char *path, int xTrans, int yTrans)
{
	auto& tile(manager.addEntity());
	tile.addComponenet<TileComponent>(srcX, srcY, xpos, ypos, xindex, yindex, 2, 2, path, xTrans, yTrans);
	tile.addGroup(groupMapText);
}

void Game::addTextImage(int srcX, int srcY, int xpos, int ypos, const char* path)
{
	auto& tile(manager.addEntity());
	tile.addComponenet<TextImageComponent>(srcX, srcY, xpos, ypos, path);
	tile.addGroup(groupMapText);
}
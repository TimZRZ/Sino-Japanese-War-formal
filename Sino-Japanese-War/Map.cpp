#include "Map.h"
#include "Game.hpp"
#include <fstream>

Map::Map()
{
}

Map::~Map()
{

}

void Map::loadMap(std::string path, int sizeX, int sizeY)
{
	/*
	char c;
	std::fstream mapFile;
	mapFile.open(path);
	*/

	int srcX, srcY;
	int xpos, ypos;

	 
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			xpos = 1.5 * 60 * x / 2;
			ypos = 52 * y;
			if (x % 2 == 1)
			{
				xpos = 1.5 * 60 * (x - 1) / 2 + 0.75 * 60;
				ypos += 0.5 * 52;
			} 
			Game::addTile(0, 0, xpos, ypos, x, y);
			Game::addBackGroundTile(xpos/4, ypos/4, xpos, ypos, x, y);
		}
	}

	Game::addTextImage(0, 0, 750, 100, "../assets/LiaoTung.png");
	Game::addTextImage(0, 0, 350, 560, "../assets/pechili.png");
	Game::addTextImage(0, 0, 1200, 400, "../assets/Korean.png");

	//mapFile.close();
}
#include "Map.h"
#include "Game.hpp"
#include <fstream>

// x∆´¿Î÷µ
#define xStart 2

Map::Map()
{
}

Map::~Map()
{

}

Vector2D xyToPos(int x, int y)
{
	Vector2D pos;
	int xpos, ypos;
	xpos = 1.5 * 60 * x / 2;
	ypos = 52 * y;
	if (x % 2 == 1)
	{
		xpos = 1.5 * 60 * (x - 1) / 2 + 0.75 * 60;
		ypos += 0.5 * 52;
	}
	pos.x = xpos;
	pos.y = ypos;
	return pos;
}


void addMapText(int x, int y, int sizeX, int sizeY, const char *path)
{
	for (int ystr = 0; ystr < sizeY; ystr++)
	{
		for (int xstr = 0; xstr < sizeX; xstr++)
		{
			Vector2D srcPos = xyToPos(xstr, ystr);
			Vector2D destPos = xyToPos(x + xstr, y + ystr);
			if (xstr == 1)
			{
				Game::addStringTile(srcPos.x * 2 + 30 * (x % 2), srcPos.y * 2 - 104 * (x % 2), destPos.x, destPos.y, x + xstr, y + ystr, path, 15, 0);
			}
			else
			{
				Game::addStringTile(srcPos.x * 2 - 30 * ((x+1) % 2), srcPos.y * 2, destPos.x, destPos.y, x + xstr, y + ystr, path, 0, 0);
			}
		}
	}
}

void Map::loadMap(std::string path, int sizeX, int sizeY)
{
	int srcX, srcY;
	int xpos, ypos;

	int liaoTungPos[2] = {17,2};

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
			Game::addTile(0, 0, xpos, ypos, x, y, 0, 0);
			Game::addBackGroundTile(xpos/2, ypos/2, xpos, ypos, x, y, 0, 0);
		}
	}

	addMapText(13, 2, 2, 7, "../assets/LiaoTung.png");
	addMapText(7, 10, 2, 7, "../assets/pechili.png");
	addMapText(27, 7, 2, 7, "../assets/Korean.png");

	//Game::addTextImage(0, 0, 750, 100, "../assets/LiaoTung.png");
	//Game::addTextImage(0, 0, 350, 560, "../assets/pechili.png");
	//Game::addTextImage(0, 0, 1200, 400, "../assets/Korean.png");

}
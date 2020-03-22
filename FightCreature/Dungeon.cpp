#include <iostream>
#include <iomanip>
#include <vector>
#include <conio.h>
#include <sstream>
#include "Dungeon.h"

int Dungeon::getWidth()
{
	return width;
}

int Dungeon::getHeight()
{
	return height;
}

bool Dungeon::isObstacle(int x, int y)
{
	return dungeonMap[y][x] == wall;
}

bool Dungeon::isBoundary(int x, int y)
{
	return x < 1 || x > width - 2 || y < 1 || y > height - 2;
}

void Dungeon::searchPath(int nowX, int nowY, int targetX, int targetY, std::vector<std::vector<bool> >& flag)
{
	dungeonMap[nowY][nowX] = floor;
	flag[nowY][nowX] = true;
	if (nowX == targetX && nowY == targetY)
	{
		return;
	}

	int dx[] = { 0, 0, -1, 1 };
	int dy[] = { -1, 1, 0, 0 };
	int dir = rand() % 4;
	while (isBoundary(nowX + dx[dir], nowY + dy[dir]) && !flag[nowY + dy[dir]][nowX + dx[dir]])
	{
		dir = rand() % 4;
	}
	searchPath(nowX + dx[dir], nowY + dy[dir], targetX, targetY, flag);
}

void Dungeon::generateTerrain(int heroX, int heroY, int creatureX, int creatureY)
{
	for (int _y = 1; _y < height - 1; _y++)
	{
		for (int _x = 1; _x < width - 1; _x++)
		{
			dungeonMap[_y][_x] = wall;
		}
	}

	std::vector<std::vector<bool> > flag(height);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			flag[i].push_back(false);
		}
	}
	searchPath(heroX, heroY, creatureX, creatureY, flag);
	searchPath(rand() % (width - 2) + 1, rand() % (height - 2) + 1, creatureX, creatureY, flag);
	flag.clear();
}

void Dungeon::generateMap()
{
	std::stringstream ss;
	ss << std::setw(width + (std::streamsize)1) << std::setfill(wall) << "\n";
	std::string stringTemp = ss.str();
	ss.str("");
	dungeonMap.push_back(stringTemp);
	for (int y = 1; y < height - 1; y++)
	{
		ss << wall << std::setw(width - (std::streamsize)1) << std::setfill(floor) << wall << "\n";
		stringTemp = ss.str();
		ss.str("");
		dungeonMap.push_back(stringTemp);
	}
	ss << std::setw(width + (std::streamsize)1) << std::setfill(wall) << "\n";
	stringTemp = ss.str();
	ss.str("");
	dungeonMap.push_back(stringTemp);
}

void Dungeon::loadMap()
{
	/*std::string inputString;
	while (true)
	{
		std::getline(cin, inputString);
		dungeonMapTemp.push_back(inputString);
	}

	height = 0;
	for (string item : dungeonMapTemp)
	{
		width = 0;
		for (char c : item)
		{
			dungeonMap[height].push_back(c);
			width++;
		}
		height++;
	}*/
}

void Dungeon::inputMap()
{
	std::cout << "Creating Map...\n";
	std::cout << "input height:";
	std::cin >> height;
	std::cout << "input width:";
	std::cin >> width;
	std::cout << "input style of wall:";
	std::cin >> wall;
	std::cout << "input style of floor:";
	std::string stringTemp;
	std::cin.ignore();
	getline(std::cin, stringTemp);
	floor = stringTemp[0];

	generateMap();
}

void Dungeon::printMap()
{
	for (std::string line : dungeonMap)
	{
		std::cout << line;
	}
}

char Dungeon::getFloor()
{
	return floor;
}

std::vector<std::string> Dungeon::outMap()
{
	return dungeonMap;
}
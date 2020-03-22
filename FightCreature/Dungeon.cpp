#include <iostream>
#include <iomanip>
#include <vector>
#include <conio.h>
#include <sstream>
#include <ctime>
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

void Dungeon::searchPath(int nowX, int nowY, int targetX, int targetY)
{
	dungeonMap[nowY][nowX] = floor;
	if (nowX == targetX && nowY == targetY)
	{
		return;
	}

	int dx[] = { 0, 0, -1, 1 };
	int dy[] = { -1, 1, 0, 0 };
	int dir = rand() % 4;
	while (isBoundary(nowX + dx[dir], nowY + dy[dir]))
	{
		dir = rand() % 4;
	}
	searchPath(nowX + dx[dir], nowY + dy[dir], targetX, targetY);
}

void Dungeon::generateTerrain()
{
	srand(time(NULL));
	for (int _y = 1; _y < height - 1; _y++)
	{
		for (int _x = 1; _x < width - 1; _x++)
		{
			dungeonMap[_y][_x] = wall;
		}
	}
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

	std::stringstream ss;
	ss << std::setw(width + (std::streamsize)1) << std::setfill(wall) << "\n";
	stringTemp = ss.str();
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
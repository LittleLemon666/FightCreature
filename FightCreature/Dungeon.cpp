#include <iostream>
#include <iomanip>
#include <vector>
#include <conio.h>
#include <string>
#include <sstream>
#include "Dungeon.h"
using namespace std;

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

bool Dungeon::searchPath(int nowX, int nowY, int targetX, int targetY, int p, int pStop)
{
	if (p >= pStop)
	{
		return false;
	}
	dungeonMap[nowY][nowX] = floor;
	if (nowX == targetX && nowY == targetY)
	{
		return true;
	}

	int dx[] = { 0, 0, -1, 1 };
	int dy[] = { -1, 1, 0, 0 };
	int dir = rand() % 4;
	while (isBoundary(nowX + dx[dir], nowY + dy[dir]))
	{
		dir = rand() % 4;
	}
	return searchPath(nowX + dx[dir], nowY + dy[dir], targetX, targetY, p + 1, pStop);
}

void Dungeon::generateTerrain(int heroX, int heroY, vector<Point> point, int creatureNum)
{
	for (int _y = 1; _y < height - 1; _y++)
	{
		for (int _x = 1; _x < width - 1; _x++)
		{
			dungeonMap[_y][_x] = wall;
		}
	}

	cout << "Loading";
	for (int i = 0; i < creatureNum; i++)
	{
		while (true)
		{
			if (searchPath(heroX, heroY, point[i].X, point[i].Y, 0, pow(heroX - point[i].X, 2) + pow(heroY - point[i].Y, 2)))
			{
				break;
			}
			cout << ".";
		}
	}
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

	cout << "Loading";
	while (true)
	{
		if (searchPath(heroX, heroY, creatureX, creatureY, 0, pow(heroX - creatureX, 2) + pow(heroY - creatureY, 2)))
		{
			break;
		}
		cout << ".";
	}
}

void Dungeon::generateMap()
{
	stringstream ss;
	ss << setw(width + (streamsize)1) << setfill(wall) << "\n";
	string stringTemp = ss.str();
	ss.str("");
	dungeonMap.push_back(stringTemp);
	for (int y = 1; y < height - 1; y++)
	{
		ss << wall << setw(width - (streamsize)1) << setfill(floor) << wall << "\n";
		stringTemp = ss.str();
		ss.str("");
		dungeonMap.push_back(stringTemp);
	}
	ss << setw(width + (streamsize)1) << setfill(wall) << "\n";
	stringTemp = ss.str();
	ss.str("");
	dungeonMap.push_back(stringTemp);
}

int Dungeon::loadMap(vector<string> lineString)
{
	stringstream ss;
	ss << lineString[0];
	ss >> width >> height >> wall >> floor;
	int lineIndex = 1;
	for (; lineIndex - 1 < height; lineIndex++)
	{
		dungeonMap.push_back(lineString[lineIndex] + "\n");
	}
	return lineIndex;
}

void Dungeon::inputMap()
{
	cout << "Creating Map...\n";
	while (true)
	{
		cout << "input height:";
		cin >> height;
		if (height <= 3)
		{
			cout << "Too low.\n";
		}
	}
	while (true)
	{
		cout << "input width:";
		cin >> width;
		if (width <= 3)
		{
			cout << "Too low.\n";
		}
	}
	cout << "input style of wall:";
	cin >> wall;
	cout << "input style of floor:";
	string stringTemp;
	cin.ignore();
	getline(cin, stringTemp);
	floor = stringTemp[0];

	generateMap();
}

void Dungeon::printMap()
{
	for (string line : dungeonMap)
	{
		cout << line;
	}
}

char Dungeon::getFloor()
{
	return floor;
}

vector<string> Dungeon::outMap()
{
	return dungeonMap;
}
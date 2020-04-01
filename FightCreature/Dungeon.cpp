#include <iostream>
#include <iomanip>
#include <vector>
#include <conio.h>
#include <string>
#include <sstream>
#include "Dungeon.h"
using namespace std;

Dungeon::Dungeon()
{

}

Dungeon::~Dungeon()
{

}

int Dungeon::getWidth()
{
	return width;
}

int Dungeon::getHeight()
{
	return height;
}

char Dungeon::getFloorSkin()
{
	return floor;
}

bool Dungeon::isObstacle(int x, int y)
{
	return dungeonMap[y][x] == wall;
}

bool Dungeon::isBoundary(int x, int y)
{
	return x < 0 || x > width - 1 || y < 0 || y > height - 1;
}

void Dungeon::fill()
{
	for (int _y = 1; _y < height - 1; _y++)
	{
		for (int _x = 1; _x < width - 1; _x++)
		{
			dungeonMap[_y][_x] = wall;
		}
	}
}

void Dungeon::generatePlain()
{
	stringstream ss;
	ss << setw(width + (streamsize)1) << setfill(wall) << "\0";
	string stringTemp = ss.str();
	ss.str("");
	dungeonMap.push_back(stringTemp);
	for (int y = 1; y < height - 1; y++)
	{
		ss << wall << setw(width - (streamsize)1) << setfill(floor) << wall << "\0";
		stringTemp = ss.str();
		ss.str("");
		dungeonMap.push_back(stringTemp);
	}
	ss << setw(width + (streamsize)1) << setfill(wall) << "\0";
	stringTemp = ss.str();
	ss.str("");
	dungeonMap.push_back(stringTemp);
}
//The map of FCF must fill with space until reaching its width.
int Dungeon::loadMap(vector<string> lineString)
{
	stringstream ss;
	ss << lineString[0];
	ss >> width >> height >> wall;
	floor = lineString[0][(int)lineString[0].length() - 1];
	int lineIndex = 1;
	for (; lineIndex - 1 < height; lineIndex++)
	{
		dungeonMap.push_back(lineString[lineIndex] + "\0");
	}
	return lineIndex;
}

void Dungeon::customMap()
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
		else
		{
			break;
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
		else
		{
			break;
		}
	}
	cout << "input style of wall:";
	cin >> wall;
	cout << "input style of floor:";
	string stringTemp;
	cin.ignore();
	getline(cin, stringTemp);
	floor = stringTemp[0];

	generatePlain();
}

void Dungeon::generateTerrain(int heroX, int heroY, vector<Point> creaturePoints, int creatureNum)
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
			if (searchPath(heroX, heroY, creaturePoints[i].X, creaturePoints[i].Y, 0, pow(heroX - creaturePoints[i].X, 2) + pow(heroY - creaturePoints[i].Y, 2)))
			{
				break;
			}
			cout << ".";
		}
	}
}

void Dungeon::generateTerrain(int heroX, int heroY, int creatureX, int creatureY)
{
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

void Dungeon::printMap()
{
	for (string line : dungeonMap)
	{
		cout << line;
	}
}

vector<string> Dungeon::outputMap()
{
	return dungeonMap;
}
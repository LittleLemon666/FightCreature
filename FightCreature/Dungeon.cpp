#include <iostream>
#include <vector>
#include <conio.h>
#include <string>
#include <sstream>
#include "Dungeon.h"
using namespace std;
int Dungeon::dungeonIndex = 0;

Dungeon::Dungeon()
{
}

Dungeon::~Dungeon()
{

}

const int Dungeon::getWidth() const
{
	return width;
}

const int Dungeon::getHeight() const
{
	return height;
}

const char Dungeon::getFloorSkin() const
{
	return floor;
}

const bool Dungeon::isObstacle(const Point position) const
{
	return dungeonMap[position.Y][position.X] == wall;
}

const bool Dungeon::isBoundary(const Point position) const
{
	return position.X < 0 || position.X > width - 1 || position.Y < 0 || position.Y > height - 1;
}

const void Dungeon::fill()
{
	for (int _y = 0; _y < height; _y++)
	{
		for (int _x = 0; _x < width; _x++)
		{
			dungeonMap[_y][_x] = wall;
		}
	}
}

const void Dungeon::generatePlain()
{
	dungeonMap.push_back(string(width, wall));
	for (int y = 1; y < height - 1; y++)
	{
		dungeonMap.push_back(string(1,wall) + string(width - 2, floor) + string(1, wall));
	}
	dungeonMap.push_back(string(width, wall));
}
//The map of FCF must fill with space until reaching its width.
const int Dungeon::loadMap(vector<string> lineString)
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

const void Dungeon::customMap()
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

const void Dungeon::generateTerrain(const Point heroPosition, vector<Point> creaturePoints, const int creatureNum)
{
	fill();

	for (int i = 0; i < creatureNum; i++)
	{
		while (true)
		{
			if (searchPath(heroPosition, creaturePoints[i], 0, pow(heroPosition.X - creaturePoints[i].X, 2) + pow(heroPosition.Y - creaturePoints[i].Y, 2)))
			{
				break;
			}
			cout << ".";
		}
	}
}

const void Dungeon::generateTerrain(const Point heroPosition, const Point creaturePosition)
{
	while (true)
	{
		if (searchPath(heroPosition, creaturePosition, 0, pow(heroPosition.X - creaturePosition.X, 2) + pow(heroPosition.Y - creaturePosition.Y, 2)))
		{
			break;
		}
		cout << ".";
	}
}

const bool Dungeon::searchPath(Point nowPosition, Point targetPosition, int p, int pStop)
{
	if (p >= pStop)
	{
		return false;
	}
	dungeonMap[nowPosition.Y][nowPosition.X] = floor;
	if (nowPosition.X == targetPosition.X && nowPosition.Y == targetPosition.Y)
	{
		return true;
	}

	int dx[] = { 0, 0, -1, 1 };
	int dy[] = { -1, 1, 0, 0 };
	int dir = rand() % 4;
	while (isBoundary(Point(nowPosition.X + dx[dir], nowPosition.Y + dy[dir])))
	{
		dir = rand() % 4;
	}
	return searchPath(Point(nowPosition.X + dx[dir], nowPosition.Y + dy[dir]), targetPosition, p + 1, pStop);
}

vector<string> Dungeon::outputMap() const
{
	return dungeonMap;
}
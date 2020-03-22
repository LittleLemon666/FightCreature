#pragma once
#include <vector>
#include <iostream>
#include "Point.h"

class Dungeon
{
private:
	int width = 15;
	int height = 15;
	char wall = '#';
	char floor = ' ';
	std::vector<std::string> dungeonMap;

public:
	/*void inputMap()
	{
		vector<string> dungeonMapTemp;
		string inputString;
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
		}
	}*/

	int getWidth();
	int getHeight();
	bool isObstacle(int x, int y);
	bool isBoundary(int x, int y);
	void generateTerrain();
	void searchPath(int nowX, int nowY, int targetX, int targetY);
	void inputMap();
	void printMap();
	char getFloor();
	std::vector<std::string> outMap();
};
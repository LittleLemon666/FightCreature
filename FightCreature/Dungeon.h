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
	int getWidth();
	int getHeight();
	bool isObstacle(int x, int y);
	bool isBoundary(int x, int y);
	void generateTerrain(int heroX, int heroY, int creatureX, int creatureY);
	void searchPath(int nowX, int nowY, int targetX, int targetY, std::vector<std::vector<bool> >& flag);
	void inputMap();
	void loadMap();
	void generateMap();
	void printMap();
	char getFloor();
	std::vector<std::string> outMap();
};
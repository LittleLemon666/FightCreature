#pragma once
#include <vector>
#include <iostream>
#include "Point.h"

class Dungeon
{
private:
	int width = 28;
	int height = 14;
	char wall = '#';
	char floor = ' ';
	std::vector<std::string> dungeonMap;

public:
	int getWidth();
	int getHeight();
	bool isObstacle(int x, int y);
	bool isBoundary(int x, int y);
	void fill();
	void generateTerrain(int heroX, int heroY, std::vector<Point> point, int creatureNum);
	void generateTerrain(int heroX, int heroY, int creatureX, int creatureY);
	bool searchPath(int nowX, int nowY, int targetX, int targetY, int p, int pStop);
	void inputMap();
	int loadMap(std::vector<std::string> lineString);
	void generateMap();
	void printMap();
	char getFloor();
	std::vector<std::string> outMap();
};
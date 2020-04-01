#pragma once
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
	Dungeon();
	~Dungeon();
	int getWidth();
	int getHeight();
	char getFloorSkin();
	bool isObstacle(int x, int y);
	bool isBoundary(int x, int y);
	void fill();
	void generatePlain();
	int loadMap(std::vector<std::string> lineString);
	void customMap();
	void generateTerrain(int heroX, int heroY, std::vector<Point> creaturePoints, int creatureNum);
	void generateTerrain(int heroX, int heroY, int creatureX, int creatureY);
	bool searchPath(int nowX, int nowY, int targetX, int targetY, int p, int pStop);
	void printMap();
	std::vector<std::string> outputMap();
};